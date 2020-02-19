///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2013, Sony Pictures Imageworks
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the
// distribution.  Neither the name of Sony Pictures Imageworks nor the
// names of its contributors may be used to endorse or promote
// products derived from this software without specific prior written
// permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////////
//
//  OpenGLRenderer.cpp
//  spReticle.1.7
//
//  Created by Henry Vera on 4/23/13.
//
//

#define GL_GLEXT_PROTOTYPES

#include <vector>
#include <algorithm>

#include "font.h"
#include "OpenGLRenderer.h"
#ifndef _WIN32
#    include "GL/glext.h"
#else
#    include "glext.h"
#endif

OpenGLRenderer::OpenGLRenderer()
{
     // Populate fontMap
     FontData *fd;
     for (int fontIndex = 0; fontIndex < NUM_FONTS; fontIndex++) {
         TextureFont *font = fonts[fontIndex];
         int size = int(font->size);
         
         // Check to see if we already have a font for this size
         FontMap::iterator it = fontMap.find(size);
         if (it == fontMap.end()) {
             fd = new FontData(size);
             fontMap[size] = fd;
         }
         else
             fd = fontMap[size];
         
         if (font->bold) {
             fd->bold = font;
             if (!fd->generic)
             fd->generic = font;
         }
         else {
             fd->normal = font;
             fd->generic = font;
         }
         
         // Populate glyphMap based on wchar_t
         for (unsigned int glyphIndex = 0; glyphIndex < font->glyphs_count;glyphIndex++) {
             TextureGlyph *glyph = &font->glyphs[glyphIndex];
             font->glyphMap[glyph->charcode] = glyph;
         }
     }
}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::prepareForDraw(float portWidth, float portHeight)
{
    // Store all of the openGL attribute settings
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
    
    // Go into 2D ortho mode
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho(
            0.0, (GLdouble) portWidth,
            0.0, (GLdouble) portHeight,
            -1, 1
            );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Turn on openGL blending for transparency
    glEnable(GL_BLEND);
    
    // Store the current blend types
    glGetIntegerv(GL_BLEND_SRC, & blendAttrs[0]);
    glGetIntegerv(GL_BLEND_DST, & blendAttrs[1]);
    
    // Set the blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Disable Depth testing
    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );

#ifndef _WIN32
    // Fix multiple light VP 2.0 text issue
    // Note: either one works below:
    // Note: currently we do not have glext.h (and libftgl.so) on Windows.
    glActiveTextureARB( GL_TEXTURE0_ARB );
    //glActiveTextureARB( GL_TEXTURE0 );
#endif
}

void OpenGLRenderer::postDraw()
{
    // Turn on z-depth test
    glDepthMask( GL_TRUE );
    glEnable( GL_DEPTH_TEST );
    
    // Turn off blending
    glDisable(GL_BLEND);
    
    // Restore blend settings
    glBlendFunc(blendAttrs[0], blendAttrs[1]);
    
    // Restore matrix
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    
    // Restore all attributes
    glPopAttrib();    
}

// Given two Geom instances, this calculates the mask area between them.
//
void OpenGLRenderer::drawMask( Geom g1, Geom g2, MColor color, bool sides, bool top=true )
{
    glBegin( GL_QUADS );
    
    glColor4f( color.r, color.g, color.b, 1-color.a );
    
    if (top)
    {
        if ( (g2.y1 - g1.y1) > EPSILON )
        {
            // Bottom Mask
            glVertex2d( g1.x1, g1.y1 );
            glVertex2d( g1.x2, g1.y1 );
            glVertex2d( g2.x2, g2.y1 );
            glVertex2d( g2.x1, g2.y1 );
        }
        
        if ( (g1.y2 - g2.y2) > EPSILON )
        {
            // Top Mask
            glVertex2d( g2.x1, g2.y2 );
            glVertex2d( g2.x2, g2.y2 );
            glVertex2d( g1.x2, g1.y2 );
            glVertex2d( g1.x1, g1.y2 );
        }
    }
    else
    {
        g1.y1 = g2.y1;
        g1.y2 = g2.y2;
    }
    
    if (sides)
    {
        // Left side mask
        if ((g2.x1 - g1.x1) > EPSILON)
        {
            glVertex2d( g1.x1, g1.y1 );
            glVertex2d( g2.x1, g2.y1 );
            glVertex2d( g2.x1, g2.y2 );
            glVertex2d( g1.x1, g1.y2 );
        }
        
        // right side mask
        if ((g1.x2 - g2.x2) > EPSILON)
        {
            glVertex2d( g2.x2, g2.y1 );
            glVertex2d( g1.x2, g1.y1 );
            glVertex2d( g1.x2, g1.y2 );
            glVertex2d( g2.x2, g2.y2 );
        }
    }
    
    glEnd();
}

// This draws a single line between the specified points.
//
void OpenGLRenderer::drawLine(double x1, double x2, double y1, double y2,
                            MColor color, bool stipple)
{
    if (stipple)
    {
        glEnable (GL_LINE_STIPPLE);
        glLineStipple(2,0x00FF);
    }
    
    glBegin( GL_LINES );
    
    glColor4f( color.r, color.g, color.b, 1-color.a );
    
    glVertex2d( x1, y1 );
    glVertex2d( x2, y2 );
    
    glEnd();
    
    if (stipple)
        glDisable (GL_LINE_STIPPLE);
    
}

// Given a Geom instance, this will draw a line connecting the points.
// The argument side determines whether the sides will be drawn (the top
// will always be drawn). The stipple argument specifies whether the line
// should be solid or dashed/stippled.
//
void OpenGLRenderer::drawLines( Geom g, MColor color, bool sides, bool stipple)
{
    if (stipple)
    {
        glEnable (GL_LINE_STIPPLE);
        glLineStipple(1,0x00FF);
    }
    
    GLenum mode = ( sides ) ? GL_LINE_LOOP : GL_LINES;
    
    glBegin( mode );
    
    glColor4f( color.r, color.g, color.b, 1-color.a );
    
    glVertex2d( g.x1, g.y1 );
    glVertex2d( g.x2, g.y1 );
    glVertex2d( g.x2, g.y2 );
    glVertex2d( g.x1, g.y2 );
    
    glEnd( );
    
    if (stipple)
        glDisable (GL_LINE_STIPPLE);
}

// This function uses a font texture atlas to draw text.
//
void OpenGLRenderer::drawText(TextData *td, double tx, double ty)
{
    double screenScaleFactor = (td->textScale) ? filmback->filmbackGeom.x/1280.0f : 1.0f;
    double fontScaleFactor = (double(td->textSize) / double(MAXFONT)) * screenScaleFactor;
    
    // Get the specified font;
    TextureFont *font = (td->textBold) ? &font_120pt_bold : &font_120pt;
    
    // Get wchar pointer to text
    const wchar_t *textPtr = td->textStr.asWChar();
    
    // Build glyph array to calculate width and then draw
    float textWidth = 0.0f;
    float textHeight = 0.0f;
    int numChars = wcslen(textPtr);
    
    std::vector<TextureGlyph> glyphs(numChars);
    std::vector<double> kerning(numChars, 0.0);
    
    for (int i = 0; i < numChars; i++) {
        GlyphMap::iterator it = font->glyphMap.find(textPtr[i]);
        if (it == font->glyphMap.end()) {
            std::cout << "Unable to find font character for '" << textPtr[i] << "' for size " << font->size << std::endl;
            return;
        }
        
        glyphs[i] = *(it->second);
        
        textWidth += glyphs[i].advance_x;
        textHeight = (std::max)(textHeight,float(glyphs[i].height));

        if (i > 0 && glyphs[i].kerning_count) {
            double kernAmount = 0.0;
            for(unsigned int kernIndex = 0; kernIndex < glyphs[i].kerning_count; kernIndex++) {
                if (glyphs[i].kerning[kernIndex].charcode == textPtr[i-1] ) {
                    kernAmount = glyphs[i].kerning[kernIndex].kerning;
                    break;
                }
            }
            kerning[i] = kernAmount;
            textWidth += kerning[i];
        }
        else
            kerning[i] = 0.0;
    }
    
    // Scale textWidth based upon the fontScaleFactor;
    textWidth *= fontScaleFactor;
    textHeight *= fontScaleFactor;
    
    // Adjust tx for text alignment
    switch (td->textAlign) {
        case 1:
            tx -= textWidth / 2.0f;
            break;
        case 2:
            tx -= textWidth;
            break;
    }
    
    // Adjust ty for text alignment
    switch (td->textVAlign) {
        case 1:
            ty -= textHeight / 2.0f;
            break;
        case 2:
            ty -= textHeight;
            break;
    }
    
    // Adjust text position to account for screen scaling
    tx += td->textPosX*screenScaleFactor;
    ty += td->textPosY*screenScaleFactor;
    
    // Actually draw the glyphs
    glColor4f( td->textColor.r, td->textColor.g, td->textColor.b, 1-td->textColor.a );
    for( int i=0; i<numChars; i++) {
        double x = double(tx + ((glyphs[i].offset_x + kerning[i]) * fontScaleFactor));
        double y = double(ty + (glyphs[i].offset_y*fontScaleFactor));
        double w  = glyphs[i].width * fontScaleFactor;
        double h  = glyphs[i].height * fontScaleFactor;
        glBegin( GL_QUADS );
        {
            glTexCoord2f( glyphs[i].u0, glyphs[i].v0 ); glVertex2d( x,   y   );
            glTexCoord2f( glyphs[i].u0, glyphs[i].v1 ); glVertex2d( x,   y-h );
            glTexCoord2f( glyphs[i].u1, glyphs[i].v1 ); glVertex2d( x+w, y-h );
            glTexCoord2f( glyphs[i].u1, glyphs[i].v0 ); glVertex2d( x+w, y   );
        }
        glEnd();
        tx += (glyphs[i].advance_x + kerning[i]) * fontScaleFactor;
        ty += glyphs[i].advance_y * fontScaleFactor;
    }
}

// Make sure everything is ready for text drawing
void OpenGLRenderer::enableTextRendering() {
    // Enable GL textures
    glEnable( GL_TEXTURE_2D );

    if (!fontAtlas.id) {
        glGenTextures( 1, &fontAtlas.id );
        glBindTexture( GL_TEXTURE_2D, fontAtlas.id );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        //Generate the texture with mipmaps
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

        glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, fontAtlas.width, fontAtlas.height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, fontAtlas.data );
    }

    glBindTexture( GL_TEXTURE_2D, fontAtlas.id );
}

// Turn off text rendering
void OpenGLRenderer::disableTextRendering() {
    // Disable GL textures
    glDisable( GL_TEXTURE_2D );
}

