/*
    Project     Segmented LED Display - ASCII Library
    @author     David Madison
    @modified   JohnnyGalto (added entry at the end of the array)
    @link       github.com/dmadison/Segmented-LED-Display-ASCII
    @license    MIT - Copyright (c) 2017 David Madison

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS= IN
   THE SOFTWARE.

*/


#ifndef FONTS_H
#define FONTS_H

#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#endif

static const byte font14s[] = {
  0x00, 0x00, /* (space) 0x00*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/ //10
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/ //20
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/ //30
  0x00, 0x00, /* spare*/
  0x00, 0x00, /* spare*/
  0x00, 0x86, /* ! */
  0x04, 0x02, /* " */
  0x25, 0x4E, /* # */
  0x25, 0x6D, /* $ */
  0x7F, 0x64, /* % */
  0x46, 0x59, /* & */
  0x04, 0x00, /* ' */
  0x18, 0x00, /* ( */ //40
  0x42, 0x00, /* ) */
  0x7F, 0x40, /* * */
  0x25, 0x40, /* + */
  0x40, 0x00, /* , */
  0x01, 0x40, /* - */
  0x80, 0x80, /* . */
  0x48, 0x00, /* / */
  0x48, 0x3F, /* 0 */ //48
  0x08, 0x06, /* 1 */
  0x01, 0x5B, /* 2 */
  0x01, 0x4F, /* 3 */
  0x01, 0x66, /* 4 */
  0x01, 0x6D, /* 5 */
  0x01, 0x7D, /* 6 */
  0x00, 0x07, /* 7 */
  0x01, 0x7F, /* 8 */
  0x01, 0x6F, /* 9 */
  0x24, 0x00, /* : */
  0x44, 0x00, /* ; */
  0x18, 0x00, /* < */
  0x01, 0x48, /* = */
  0x43, 0x40, /* > */
  0xA1, 0x23, /* ? */
  0x05, 0x3B, /* @ */
  0x01, 0x77, /* A */
  0x25, 0x0F, /* B */
  0x00, 0x39, /* C */
  0x24, 0x0F, /* D */
  0x00, 0x79, /* E */
  0x00, 0x71, /* F */
  0x01, 0x3D, /* G */
  0x01, 0x76, /* H */
  0x24, 0x09, /* I */
  0x00, 0x1E, /* J */
  0x18, 0x70, /* K */
  0x00, 0x38, /* L */
  0x0A, 0x36, /* M */
  0x12, 0x36, /* N */
  0x00, 0x3F, /* O */
  0x01, 0x73, /* P */
  0x10, 0x3F, /* Q */
  0x11, 0x73, /* R */
  0x03, 0x0D, /* S */
  0x24, 0x01, /* T */
  0x00, 0x3E, /* U */
  0x48, 0x30, /* V */
  0x50, 0x36, /* W */
  0x5A, 0x00, /* X */
  0x01, 0x6E, /* Y */
  0x48, 0x09, /* Z */
  0x00, 0x39, /* [ */
  0x12, 0x00, /* \ */
  0x00, 0x0F, /* ] */
  0x50, 0x00, /* ^ */
  0x00, 0x08, /* _ */
  0x02, 0x00, /* ` */
  0x20, 0x58, /* a */
  0x10, 0x78, /* b */
  0x00, 0x58, /* c */
  0x41, 0x0E, /* d */
  0x40, 0x58, /* e */
  0x28, 0x40, /* f */
  0x09, 0x0E, /* g */
  0x20, 0x70, /* h */
  0x20, 0x00, /* i */
  0x44, 0x10, /* j */
  0x3C, 0x00, /* k */
  0x00, 0x30, /* l */
  0x21, 0x54, /* m */
  0x20, 0x50, /* n */
  0x01, 0x5C, /* o */
  0x02, 0x70, /* p */
  0x09, 0x06, /* q */
  0x00, 0x50, /* r */
  0x11, 0x08, /* s */
  0x00, 0x78, /* t */
  0x00, 0x1C, /* u */
  0x40, 0x10, /* v */
  0x50, 0x14, /* w */
  0x5A, 0x00, /* x */
  0x05, 0x0E, /* y */
  0x40, 0x48, /* z */
  0x42, 0x49, /* { */
  0x24, 0x00, /* | */
  0x19, 0x09, /* } */
  0x49, 0x40, /* ~ */
  0xFF, 0xFF, /* All Segments */
  0x00, 0x01, /* A  Segment */
  0x00, 0x02, /* B  Segment */
  0x00, 0x04, /* C  Segment */
  0x00, 0x08, /* D  Segment */
  0x00, 0x10, /* E  Segment */
  0x00, 0x20, /* F  Segment */
  0x00, 0x40, /* G1 Segment */
  0x00, 0x80, /* dp Segment */
  0x01, 0x00, /* G2 Segment */
  0x02, 0x00, /* H  Segment */
  0x04, 0x00, /* J  Segment */
  0x08, 0x00, /* K  Segment */
  0x10, 0x00, /* L  Segment */
  0x20, 0x00, /* M  Segment */
  0x40, 0x00, /* N  Segment */
  0x80, 0x00, /* dp Segment */
  0x49, 0x06, /* А  dec= \144 hex= \0x90 num_elem  112 */
  0x10, 0x79, /* Б */
  0x25, 0x0F, /* В */
  0x00, 0x31, /* Г */
  0x09, 0x56, /* Д */
  0x00, 0x79, /* Е */
  0x7E, 0x00, /* Ж */
  0x18, 0x49, /* З */
  0x48, 0x36, /* И */
  0x48, 0x36, /* Й */
  0x18, 0x70, /* К */
  0x48, 0x06, /* Л */
  0x0A, 0x36, /* М */
  0x01, 0x76, /* Н */
  0x00, 0x3F, /* О */
  0x00, 0x37, /* П */
  0x01, 0x73, /* Р */
  0x00, 0x39, /* С */
  0x24, 0x01, /* Т */
  0x4A, 0x00, /* У */
  0x25, 0x63, /* Ф */
  0x5A, 0x00, /* Х */
  0x24, 0x38, /* Ц */
  0x03, 0x06, /* Ч */
  0x24, 0x3E, /* Ш */
  0x05, 0x66, /* Щ */
  0x01, 0x7C, /* Ь */
  0x20, 0x7E, /* Ы */
  0x25, 0x0D, /* Ъ */
  0x42, 0x40, /* Э */
  0x19, 0x76, /* Ю */
  0x41, 0x67, /* Я */
  0x20, 0x58, /* а */
  0x12, 0x59, /* б */
  0x12, 0x78, /* в */
  0x21, 0x00, /* г */
  0x21, 0x0F, /* д */
  0x40, 0x58, /* е */
  0x7E, 0x00, /* ж */
  0x18, 0x49, /* з */
  0x60, 0x10, /* и */
  0x60, 0x10, /* й */
  0x18, 0x70, /* к */
  0x60, 0x00, /* л */
  0x0A, 0x36, /* м */
  0x01, 0x76, /* н */
  0x01, 0x5C, /* о */
  0x01, 0x54, /* п  dec= \191 hex= \0xBF num_elem 159*/
  0x01, 0x73, /* р  dec= \192 hex= \0xC0 num_elem 160*/
  0x01, 0x58, /* с */
  0x21, 0x40, /* т */
  0x4A, 0x00, /* у */
  0x25, 0x63, /* ф */
  0x5A, 0x00, /* х */
  0x24, 0x38, /* ц */
  0x03, 0x06, /* ч */
  0x24, 0x3E, /* ш */
  0x05, 0x66, /* щ */
  0x01, 0x7C, /* ь */
  0x20, 0x7E, /* ы */
  0x25, 0x0D, /* ъ */
  0x42, 0x40, /* э */
  0x19, 0x76, /* ю */
  0x41, 0x67, /* я   dec= \207 hex= \0xCF num_elem  175 */
  0x00, 0x79, /* Ё   dec= \208 hex= \0xD0 num_elem  176 */
  0x40, 0x58, /* ё   dec= \209 hex= \0xD1 num_elem  177 */
  0x01, 0x63, /*gradus     dec= \210 hex= \0xD2 num_elem  178 */
  0x00, 0x41  /* SEG A + SEG G dec= \211 hex= \0xD3 num_elem  179 */
};

/*
    This is the 'classic' fixed-space bitmap font for Adafruit_GFX since 1.0.
    See gfxfont.h for newer custom bitmap font info.
    Standard ASCII 5x7 font
*/

static const byte font5x7[] = {
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
  0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
  0x18, 0x3C, 0x7E, 0x3C, 0x18,
  0x1C, 0x57, 0x7D, 0x57, 0x1C,
  0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
  0x00, 0x18, 0x3C, 0x18, 0x00,
  0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
  0x00, 0x18, 0x24, 0x18, 0x00,
  0xFF, 0xE7, 0xDB, 0xE7, 0xFF, //10
  0x30, 0x48, 0x3A, 0x06, 0x0E,
  0x26, 0x29, 0x79, 0x29, 0x26,
  0x40, 0x7F, 0x05, 0x05, 0x07,
  0x40, 0x7F, 0x05, 0x25, 0x3F,
  0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
  0x7F, 0x3E, 0x1C, 0x1C, 0x08,
  0x08, 0x1C, 0x1C, 0x3E, 0x7F,
  0x14, 0x22, 0x7F, 0x22, 0x14,
  0x5F, 0x5F, 0x00, 0x5F, 0x5F,
  0x06, 0x09, 0x7F, 0x01, 0x7F, //20
  0x00, 0x66, 0x89, 0x95, 0x6A,
  0x60, 0x60, 0x60, 0x60, 0x60,
  0x94, 0xA2, 0xFF, 0xA2, 0x94,
  0x08, 0x04, 0x7E, 0x04, 0x08,
  0x10, 0x20, 0x7E, 0x20, 0x10,
  0x08, 0x08, 0x2A, 0x1C, 0x08,
  0x08, 0x1C, 0x2A, 0x08, 0x08,
  0x1E, 0x10, 0x10, 0x10, 0x10,
  0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
  0x30, 0x38, 0x3E, 0x38, 0x30, //30
  0x06, 0x0E, 0x3E, 0x0E, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x5F, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x07, 0x00,
  0x14, 0x7F, 0x14, 0x7F, 0x14,
  0x24, 0x2A, 0x7F, 0x2A, 0x12,
  0x23, 0x13, 0x08, 0x64, 0x62,
  0x36, 0x49, 0x56, 0x20, 0x50,
  0x00, 0x08, 0x07, 0x03, 0x00,
  0x00, 0x1C, 0x22, 0x41, 0x00, //40
  0x00, 0x41, 0x22, 0x1C, 0x00,
  0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
  0x08, 0x08, 0x3E, 0x08, 0x08,
  0x00, 0x80, 0x70, 0x30, 0x00,
  0x08, 0x08, 0x08, 0x08, 0x08,
  0x00, 0x00, 0x60, 0x60, 0x00,
  0x20, 0x10, 0x08, 0x04, 0x02,
  0x3E, 0x7F, 0x41, 0x7F, 0x3E, // Code for char 0 big   5x7 48
  0x00, 0x44, 0x7E, 0x7F, 0x40, // Code for char 1 big   5x7
  0x62, 0x73, 0x59, 0x4F, 0x66, // Code for char 2 big   5x7
  0x22, 0x63, 0x49, 0x7F, 0x36, // Code for char 3 big   5x7
  0x18, 0x1C, 0x12, 0x7F, 0x7F, // Code for char 4 big   5x7
  0x2F, 0x6F, 0x45, 0x7D, 0x39, // Code for char 5 big   5x7
  0x3E, 0x7F, 0x49, 0x7B, 0x32, // Code for char 6 big   5x7
  0x03, 0x71, 0x79, 0x0F, 0x07, // Code for char 7 big   5x7
  0x36, 0x7F, 0x49, 0x7F, 0x36, // Code for char 8 big   5x7
  0x26, 0x6F, 0x49, 0x7F, 0x3E, // Code for char 9 big   5x7 57
  0x00, 0x00, 0x14, 0x00, 0x00,
  0x00, 0x40, 0x34, 0x00, 0x00,
  0x00, 0x08, 0x14, 0x22, 0x41,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x00, 0x41, 0x22, 0x14, 0x08,
  0x02, 0x01, 0x59, 0x09, 0x06,
  0x3E, 0x41, 0x5D, 0x59, 0x4E,
  0x7C, 0x12, 0x11, 0x12, 0x7C, // A 65
  0x7F, 0x49, 0x49, 0x49, 0x36, // B
  0x3E, 0x41, 0x41, 0x41, 0x22, // C
  0x7F, 0x41, 0x41, 0x41, 0x3E, // D
  0x7F, 0x49, 0x49, 0x49, 0x41, // E
  0x7F, 0x09, 0x09, 0x09, 0x01, // F
  0x3E, 0x41, 0x41, 0x51, 0x73, // G
  0x7F, 0x08, 0x08, 0x08, 0x7F, // H
  0x00, 0x41, 0x7F, 0x41, 0x00, // I
  0x20, 0x40, 0x41, 0x3F, 0x01, // J
  0x7F, 0x08, 0x14, 0x22, 0x41, // K
  0x7F, 0x40, 0x40, 0x40, 0x40, // L
  0x7F, 0x02, 0x1C, 0x02, 0x7F, // M
  0x7F, 0x04, 0x08, 0x10, 0x7F, // N
  0x3E, 0x41, 0x41, 0x41, 0x3E, // O
  0x7F, 0x09, 0x09, 0x09, 0x06, // P
  0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
  0x7F, 0x09, 0x19, 0x29, 0x46, // R
  0x26, 0x49, 0x49, 0x49, 0x32, // S
  0x03, 0x01, 0x7F, 0x01, 0x03, // T
  0x3F, 0x40, 0x40, 0x40, 0x3F, // U
  0x1F, 0x20, 0x40, 0x20, 0x1F, // V
  0x3F, 0x40, 0x38, 0x40, 0x3F, // W
  0x63, 0x14, 0x08, 0x14, 0x63, // X
  0x03, 0x04, 0x78, 0x04, 0x03, // Y
  0x61, 0x59, 0x49, 0x4D, 0x43, // Z 90
  0x00, 0x7F, 0x41, 0x41, 0x41,
  0x02, 0x04, 0x08, 0x10, 0x20,
  0x00, 0x41, 0x41, 0x41, 0x7F,
  0x04, 0x02, 0x01, 0x02, 0x04,
  0x40, 0x40, 0x40, 0x40, 0x40,
  0x00, 0x03, 0x07, 0x08, 0x00,
  0x20, 0x54, 0x54, 0x78, 0x40, // a 97
  0x7F, 0x28, 0x44, 0x44, 0x38, // b
  0x38, 0x44, 0x44, 0x44, 0x28, // c
  0x38, 0x44, 0x44, 0x28, 0x7F, // d
  0x38, 0x54, 0x54, 0x54, 0x18, // e
  0x00, 0x08, 0x7E, 0x09, 0x02, // f
  0x18, 0xA4, 0xA4, 0x9C, 0x78, // g
  0x7F, 0x08, 0x04, 0x04, 0x78, // h
  0x00, 0x44, 0x7D, 0x40, 0x00, // i
  0x20, 0x40, 0x40, 0x3D, 0x00, // j
  0x7F, 0x10, 0x28, 0x44, 0x00, // k
  0x00, 0x41, 0x7F, 0x40, 0x00, // l
  0x7C, 0x04, 0x78, 0x04, 0x78, // m
  0x7C, 0x08, 0x04, 0x04, 0x78, // n
  0x38, 0x44, 0x44, 0x44, 0x38, // o
  0xFC, 0x18, 0x24, 0x24, 0x18, // p
  0x18, 0x24, 0x24, 0x18, 0xFC, // q
  0x7C, 0x08, 0x04, 0x04, 0x08, // r
  0x48, 0x54, 0x54, 0x54, 0x24, // s
  0x04, 0x04, 0x3F, 0x44, 0x24, // t
  0x3C, 0x40, 0x40, 0x20, 0x7C, // u
  0x1C, 0x20, 0x40, 0x20, 0x1C, // v
  0x3C, 0x40, 0x30, 0x40, 0x3C, // w
  0x44, 0x28, 0x10, 0x28, 0x44, // x
  0x4C, 0x90, 0x90, 0x90, 0x7C, // y
  0x44, 0x64, 0x54, 0x4C, 0x44, // z 122
  0x00, 0x08, 0x36, 0x41, 0x00, // 123
  0x00, 0x00, 0x77, 0x00, 0x00, // 124
  0x00, 0x41, 0x36, 0x08, 0x00, // 125
  0x02, 0x01, 0x02, 0x04, 0x02, // 126
  0x3C, 0x26, 0x23, 0x26, 0x3C, // 127
  0x38, 0x44, 0x38, 0x00, 0x00, // Code for char 0 small 3x5 128
  0x10, 0x78, 0x7C, 0x00, 0x00, // Code for char 1 small 3x5
  0x6C, 0x64, 0x5C, 0x00, 0x00, // Code for char 2 small 3x5
  0x44, 0x54, 0x6C, 0x00, 0x00, // Code for char 3 small 3x5
  0x30, 0x28, 0x7C, 0x00, 0x00, // Code for char 4 small 3x5
  0x5C, 0x54, 0x64, 0x00, 0x00, // Code for char 5 small 3x5
  0x78, 0x54, 0x74, 0x00, 0x00, // Code for char 6 small 3x5
  0x64, 0x14, 0x0C, 0x00, 0x00, // Code for char 7 small 3x5
  0x6C, 0x54, 0x6C, 0x00, 0x00, // Code for char 8 small 3x5
  0x5C, 0x54, 0x3C, 0x00, 0x00, // Code for char 9 small 3x5 137
  0x00, 0x04, 0x04, 0x04, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x3E, 0x00, 0x00, 0x00, 0x00,
  0x3E, 0x3E, 0x00, 0x00, 0x00,
  0x3E, 0x3E, 0x00, 0x3E, 0x00,
  0x3E, 0x3E, 0x00, 0x3E, 0x3E,
  0x80, 0x80, 0x80, 0x80, 0x80,
  0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
  0xD0, 0xD0, 0xD0, 0xD0, 0xD0,
  0xD8, 0xD8, 0xD8, 0xD8, 0xD8,
  0xDA, 0xDA, 0xDA, 0xDA, 0xDA,
  0xDB, 0xDB, 0xDB, 0xDB, 0xDB,
  0x40, 0x00, 0x40, 0x00, 0x40,
  0x60, 0x00, 0x40, 0x00, 0x40,
  0x60, 0x00, 0x70, 0x00, 0x40,
  0x60, 0x00, 0x70, 0x00, 0x78,
  0x7C, 0x00, 0x40, 0x00, 0x40,
  0x7C, 0x00, 0x7E, 0x00, 0x40,
  0x7C, 0x00, 0x7E, 0x00, 0x7F,
  0x1C, 0x77, 0x41, 0x41, 0x41,
  0x41, 0x41, 0x41, 0x41, 0x41,
  0x41, 0x41, 0x41, 0x7F, 0x00,
  0x1C, 0x77, 0x41, 0x5D, 0x5D,
  0x41, 0x41, 0x41, 0x5D, 0x5D,
  0x5D, 0x5D, 0x41, 0x5D, 0x5D,
  0x5D, 0x5D, 0x41, 0x7F, 0x00,
  0x22, 0x1C, 0x14, 0x1C, 0x22,
  0x00, 0x08, 0x1C, 0x08, 0x00,
  0x00, 0x00, 0x77, 0x00, 0x00,
  0x46, 0x5D, 0x55, 0x5D, 0x31,
  0x7C, 0x55, 0x54, 0x55, 0x44,
  0x08, 0x08, 0x2A, 0x08, 0x08,
  0x00, 0x14, 0x08, 0x14, 0x00,
  0x08, 0x14, 0x22, 0x08, 0x14,
  0x7F, 0x41, 0x71, 0x31, 0x1F,
  0x03, 0x05, 0x7F, 0x05, 0x03,
  0x22, 0x14, 0x7F, 0x55, 0x22,
  0x02, 0x55, 0x7D, 0x05, 0x02,
  0x06, 0x09, 0x09, 0x06, 0x00, //gradus 176
  0x44, 0x44, 0x5F, 0x44, 0x44,
  0x1C, 0x14, 0x1C, 0x22, 0x7F,
  0x20, 0x3E, 0x61, 0x3E, 0x20,
  0x20, 0x50, 0x3F, 0x02, 0x0C,
  0x80, 0x7C, 0x20, 0x3C, 0x40,
  0x44, 0x3C, 0x04, 0x7C, 0x44,
  0x00, 0x00, 0x08, 0x00, 0x00,
  0x38, 0x55, 0x54, 0x55, 0x18,
  0x7E, 0x08, 0x10, 0x7F, 0x01,
  0x08, 0x10, 0x08, 0x04, 0x02,
  0x14, 0x08, 0x22, 0x14, 0x08,
  0x0E, 0x06, 0x0A, 0x10, 0x20,
  0x20, 0x10, 0x0A, 0x06, 0x0E,
  0x38, 0x30, 0x28, 0x04, 0x02,
  0x02, 0x04, 0x28, 0x30, 0x38,
  0x7E, 0x11, 0x11, 0x11, 0x7E, // А 192
  0x7F, 0x49, 0x49, 0x49, 0x31, // Б
  0x7F, 0x49, 0x49, 0x49, 0x36, // В
  0x7F, 0x01, 0x01, 0x01, 0x03, // Г
  0xC0, 0x7F, 0x41, 0x7F, 0xC0, // Д
  0x7F, 0x49, 0x49, 0x49, 0x41, // Е
  0x77, 0x08, 0x7F, 0x08, 0x77, // Ж
  0x41, 0x49, 0x49, 0x49, 0x36, // З
  0x7F, 0x10, 0x08, 0x04, 0x7F, // И 200
  0x7C, 0x21, 0x12, 0x09, 0x7C, // Й
  0x7F, 0x08, 0x14, 0x22, 0x41, // К
  0x40, 0x3E, 0x01, 0x01, 0x7F, // Л
  0x7F, 0x02, 0x0C, 0x02, 0x7F, // М
  0x7F, 0x08, 0x08, 0x08, 0x7F, // Н
  0x3E, 0x41, 0x41, 0x41, 0x3E, // О
  0x7F, 0x01, 0x01, 0x01, 0x7F, // П
  0x7F, 0x09, 0x09, 0x09, 0x06, // Р
  0x3E, 0x41, 0x41, 0x41, 0x22, // С
  0x01, 0x01, 0x7F, 0x01, 0x01, // Т 210
  0x07, 0x48, 0x48, 0x48, 0x3F, // У
  0x0E, 0x11, 0x7F, 0x11, 0x0E, // Ф
  0x63, 0x14, 0x08, 0x14, 0x63, // Х
  0x7F, 0x40, 0x40, 0x7F, 0xC0, // Ц
  0x07, 0x08, 0x08, 0x08, 0x7F, // Ч
  0x7F, 0x40, 0x7F, 0x40, 0x7F, // Ш
  0x7F, 0x40, 0x7F, 0x40, 0xFF, // Щ
  0x01, 0x7F, 0x48, 0x48, 0x30, // Ь
  0x7F, 0x48, 0x48, 0x30, 0x7F, // Ы
  0x7F, 0x48, 0x48, 0x48, 0x30, // Ъ 220
  0x22, 0x41, 0x49, 0x49, 0x3E, // Э
  0x7F, 0x08, 0x3E, 0x41, 0x3E, // Ю
  0x46, 0x29, 0x19, 0x09, 0x7F, // Я 223
  0x20, 0x54, 0x54, 0x54, 0x78, // а 224
  0x3C, 0x4A, 0x4A, 0x49, 0x31, // б
  0x7C, 0x54, 0x54, 0x54, 0x28, // в
  0x7C, 0x04, 0x04, 0x04, 0x0C, // г
  0xC0, 0x78, 0x44, 0x7C, 0xC0, // д
  0x38, 0x54, 0x54, 0x54, 0x18, // е
  0x6C, 0x10, 0x7C, 0x10, 0x6C, // ж
  0x44, 0x54, 0x54, 0x54, 0x28, // з
  0x7C, 0x20, 0x10, 0x08, 0x7C, // и
  0x7C, 0x40, 0x26, 0x10, 0x7C, // й
  0x7C, 0x10, 0x10, 0x28, 0x44, // к
  0x40, 0x38, 0x04, 0x04, 0x7C, // л
  0x7C, 0x08, 0x10, 0x08, 0x7C, // м
  0x7C, 0x10, 0x10, 0x10, 0x7C, // н
  0x38, 0x44, 0x44, 0x44, 0x38, // о
  0x7C, 0x04, 0x04, 0x04, 0x7C, // п
  0x7C, 0x14, 0x14, 0x14, 0x08, // р
  0x38, 0x44, 0x44, 0x44, 0x48, // с
  0x04, 0x04, 0x7C, 0x04, 0x04, // т
  0x0C, 0x50, 0x50, 0x50, 0x3C, // у
  0x18, 0x24, 0xFC, 0x24, 0x18, // ф
  0x44, 0x28, 0x10, 0x28, 0x44, // х
  0x7C, 0x40, 0x40, 0x7C, 0xC0, // ц
  0x0C, 0x10, 0x10, 0x10, 0x7C, // ч
  0x7C, 0x40, 0x7C, 0x40, 0x7C, // ш
  0x7C, 0x40, 0x7C, 0x40, 0xFC, // щ
  0x04, 0x7C, 0x50, 0x50, 0x20, // ь
  0x7C, 0x50, 0x50, 0x20, 0x7C, // ы
  0x7C, 0x50, 0x50, 0x50, 0x20, // ъ
  0x28, 0x44, 0x54, 0x54, 0x38, // э
  0x7C, 0x10, 0x38, 0x44, 0x38, // ю
  0x48, 0x34, 0x14, 0x14, 0x7C  // я 255
};

static const byte font14s2[] = {
  0x00, 0x00, /* (space) */
  0x00, 0x86, /* ! */
  0x02, 0x02, /* " */
  0x12, 0xCE, /* # */
  0x12, 0xED, /* $ */
  0x3F, 0xE4, /* % */
  0x23, 0x59, /* & */
  0x02, 0x00, /* ' */
  0x0C, 0x00, /* ( */
  0x21, 0x00, /* ) */
  0x3F, 0xC0, /* * */
  0x12, 0xC0, /* + */
  0x20, 0x00, /* , */
  0x00, 0xC0, /* - */
  0x00, 0x80, /* . */
  0x24, 0x00, /* / */
  0x24, 0x3F, /* 0 */
  0x04, 0x06, /* 1 */
  0x00, 0xDB, /* 2 */
  0x00, 0xCF, /* (008F) 3 */
  0x00, 0xE6, /* 4 */
  0x00, 0xED, /* (2069) 5 */
  0x00, 0xFD, /* 6 */
  0x00, 0x07, /* 7 */
  0x00, 0xFF, /* 8 */
  0x00, 0xEF, /* 9 */
  0x12, 0x00, /* : */
  0x22, 0x00, /* ; */
  0x0C, 0x40, /* < */
  0x00, 0xC8, /* = */
  0x21, 0x80, /* > */
  0x50, 0x83, /* ? */
  0x02, 0xBB, /* @ */
  0x00, 0xF7, /* A */
  0x12, 0x8F, /* B */
  0x00, 0x39, /* C */
  0x12, 0x0F, /* D */
  0x00, 0x79, /* E */
  0x00, 0x71, /* F */
  0x00, 0xBD, /* G */
  0x00, 0xF6, /* H */
  0x12, 0x09, /* I */
  0x00, 0x1E, /* J */
  0x0C, 0x70, /* K */
  0x00, 0x38, /* L */
  0x05, 0x36, /* M */
  0x09, 0x36, /* N */
  0x00, 0x3F, /* O */
  0x00, 0xF3, /* P */
  0x08, 0x3F, /* Q */
  0x08, 0xF3, /* R */
  0x01, 0x8D, /* S */
  0x12, 0x01, /* T */
  0x00, 0x3E, /* U */
  0x24, 0x30, /* V */
  0x28, 0x36, /* W */
  0x2D, 0x00, /* X */
  0x00, 0xEE, /* Y */
  0x24, 0x09, /* Z */
  0x00, 0x39, /* [ */
  0x09, 0x00, /* \ */
  0x00, 0x0F, /* ] */
  0x28, 0x00, /* ^ */
  0x00, 0x08, /* _ */
  0x01, 0x00, /* ` */
  0x10, 0x58, /* a */
  0x08, 0x78, /* b */
  0x00, 0xD8, /* c */
  0x20, 0x8E, /* d */
  0x20, 0x58, /* e */
  0x14, 0xC0, /* f */
  0x04, 0x8E, /* g */
  0x10, 0x70, /* h */
  0x10, 0x00, /* i */
  0x22, 0x10, /* j */
  0x1E, 0x00, /* k */
  0x00, 0x30, /* l */
  0x10, 0xD4, /* m */
  0x10, 0x50, /* n */
  0x00, 0xDC, /* o */
  0x01, 0x70, /* p */
  0x04, 0x86, /* q */
  0x00, 0x50, /* r */
  0x08, 0x88, /* s */
  0x00, 0x78, /* t */
  0x00, 0x1C, /* u */
  0x20, 0x10, /* v */
  0x28, 0x14, /* w */
  0x2D, 0x00, /* x */
  0x02, 0x8E, /* y */
  0x20, 0x48, /* z */
  0x21, 0x49, /* { */
  0x12, 0x00, /* | */
  0x0C, 0x89, /* } */
  0x24, 0xC0, /* ~ */
  0x00, 0x00, /* (del) */
  0xFF, 0xFF, /* All Segments */
  0x00, 0x01, /* A  Segment */
  0x00, 0x02, /* B  Segment */
  0x00, 0x04, /* C  Segment */
  0x00, 0x08, /* D  Segment */
  0x00, 0x10, /* E  Segment */
  0x00, 0x20, /* F  Segment */
  0x00, 0x40, /* G1 Segment */
  0x00, 0x80, /* G2 Segment */
  0x01, 0x00, /* H  Segment */
  0x02, 0x00, /* J  Segment */
  0x04, 0x00, /* K  Segment */
  0x08, 0x00, /* L  Segment */
  0x10, 0x00, /* M  Segment */
  0x20, 0x00  /* N  Segment */
};

#endif // FONTS_H
