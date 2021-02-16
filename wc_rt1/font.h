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




const unsigned int FourteenSegmentASCII[180] = {
  0x0000, /* (space) 0x00*/
  0x0086, /* ! */
  0x0402, /* " */
  0x254E, /* # */
  0x256D, /* $ */
  0x7F64, /* % */
  0x4659, /* & */
  0x0400, /* ' */
  0x1800, /* ( */
  0x4200, /* ) */
  0x7F40, /* * */
  0x2540, /* + */
  0x4000, /* , */
  0x0140, /* - */
  0x8080, /* . */
  0x4800, /* / */
  0x483F, /* 0 */
  0x0806, /* 1 */
  0x015B, /* 2 */
  0x014F, /* 3 */
  0x0166, /* 4 */
  0x016D, /* 5 */
  0x017D, /* 6 */
  0x0007, /* 7 */
  0x017F, /* 8 */
  0x016F, /* 9 */
  0x2400, /* : */
  0x4400, /* ; */
  0x1800, /* < */
  0x0148, /* = */
  0x4340, /* > */
  0xA123, /* ? */
  0x053B, /* @ */
  0x0177, /* A */
  0x250F, /* B */
  0x0039, /* C */
  0x240F, /* D */
  0x0079, /* E */
  0x0071, /* F */
  0x013D, /* G */
  0x0176, /* H */
  0x2409, /* I */
  0x001E, /* J */
  0x1870, /* K */
  0x0038, /* L */
  0x0A36, /* M */
  0x1236, /* N */
  0x003F, /* O */
  0x0173, /* P */
  0x103F, /* Q */
  0x1173, /* R */
  0x030D, /* S */
  0x2401, /* T */
  0x003E, /* U */
  0x4830, /* V */
  0x5036, /* W */
  0x5A00, /* X */
  0x016E, /* Y */
  0x4809, /* Z */
  0x0039, /* [ */
  0x1200, /* \ */
  0x000F, /* ] */
  0x5000, /* ^ */
  0x0008, /* _ */
  0x0200, /* ` */
  0x2058, /* a */
  0x1078, /* b */
  0x0058, /* c */
  0x410E, /* d */
  0x4058, /* e */
  0x2840, /* f */
  0x090E, /* g */
  0x2070, /* h */
  0x2000, /* i */
  0x4410, /* j */
  0x3C00, /* k */
  0x0030, /* l */
  0x2154, /* m */
  0x2050, /* n */
  0x015C, /* o */
  0x0270, /* p */
  0x0906, /* q */
  0x0050, /* r */
  0x1108, /* s */
  0x0078, /* t */
  0x001C, /* u */
  0x4010, /* v */
  0x5014, /* w */
  0x5A00, /* x */
  0x050E, /* y */
  0x4048, /* z */
  0x4249, /* { */
  0x2400, /* | */
  0x1909, /* } */
  0x4940, /* ~ */
  0xFFFF, /* All Segments */
  0x0001, /* A  Segment */
  0x0002, /* B  Segment */
  0x0004, /* C  Segment */
  0x0008, /* D  Segment */
  0x0010, /* E  Segment */
  0x0020, /* F  Segment */
  0x0040, /* G1 Segment */
  0x0080, /* dp Segment */
  0x0100, /* G2 Segment */
  0x0200, /* H  Segment */
  0x0400, /* J  Segment */
  0x0800, /* K  Segment */
  0x1000, /* L  Segment */
  0x2000, /* M  Segment */
  0x4000, /* N  Segment */
  0x8000, /* dp Segment */
  0x4906, /* А  dec= \144 hex= \0x90 num_elem  112 */
  0x1079, /* Б */
  0x250F, /* В */
  0x0031, /* Г */
  0x0956, /* Д */
  0x0079, /* Е */
  0x7E00, /* Ж */
  0x1849, /* З */
  0x4836, /* И */
  0x4836, /* Й */
  0x1870, /* К */
  0x4806, /* Л */
  0x0A36, /* М */
  0x0176, /* Н */
  0x003F, /* О */
  0x0037, /* П */
  0x0173, /* Р */
  0x0039, /* С */
  0x2401, /* Т */
  0x4A00, /* У */
  0x2563, /* Ф */
  0x5A00, /* Х */
  0x2438, /* Ц */
  0x0306, /* Ч */
  0x243E, /* Ш */
  0x0566, /* Щ */
  0x017C, /* Ь */
  0x207E, /* Ы */
  0x250D, /* Ъ */
  0x4240, /* Э */
  0x1976, /* Ю */
  0x4167, /* Я */
  0x2058, /* а */
  0x1259, /* б */
  0x1278, /* в */
  0x2100, /* г */
  0x210F, /* д */
  0x4058, /* е */
  0x7E00, /* ж */
  0x1849, /* з */
  0x6010, /* и */
  0x6010, /* й */
  0x1870, /* к */
  0x6000, /* л */
  0x0A36, /* м */
  0x0176, /* н */
  0x015C, /* о */
  0x0154, /* п  dec= \191 hex= \0xBF num_elem 159*/
  0x0173, /* р  dec= \192 hex= \0xC0 num_elem 160*/
  0x0158, /* с */
  0x2140, /* т */
  0x4A00, /* у */
  0x2563, /* ф */
  0x5A00, /* х */
  0x2438, /* ц */
  0x0306, /* ч */
  0x243E, /* ш */
  0x0566, /* щ */
  0x017C, /* ь */
  0x207E, /* ы */
  0x250D, /* ъ */
  0x4240, /* э */
  0x1976, /* ю */
  0x4167, /* я   dec= \207 hex= \0xCF num_elem  175 */
  0x0079, /* Ё   dec= \208 hex= \0xD0 num_elem  176 */
  0x4058, /* ё   dec= \209 hex= \0xD1 num_elem  177 */
  0x0163, /*gradus     dec= \210 hex= \0xD2 num_elem  178 */
  0x0041  /* SEG A + SEG G dec= \211 hex= \0xD3 num_elem  179 */
};
