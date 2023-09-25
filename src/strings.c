/*
 * MIT License
 *
 * Copyright (c) 2023 Karol Zimmer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "strings.h"
#include "../res/res.h"

STRING strCm, strKg, strFt, strLbs,
	strValue, strIncorrect;

extern HINSTANCE hInst;

#define LOAD_STR(a, b) LoadStringW(hInst, a, b, sizeof(STRING)/sizeof(WCHAR))

void BC2StringsLoad()
{

	LOAD_STR(IDS_CM, strCm);
	LOAD_STR(IDS_KG, strKg);
	LOAD_STR(IDS_FT, strFt);
	LOAD_STR(IDS_LBS, strLbs);
	LOAD_STR(IDS_VALUE, strValue);
	LOAD_STR(IDS_INCORRECT, strIncorrect);

}
