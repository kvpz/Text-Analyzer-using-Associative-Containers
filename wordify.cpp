/*
  Kevin Perez
  6/23/15

  Wordify() is used to extract a word from a string with the following conditions:
Wordify rules
-------------

leading characters:
  skip ch until one of the following is true:
   ch is the null character
   ch is a letter (ascii codes 65-90/ 97-122) convert to lower
   ch is a digit (ascii codes 48-57)
   ch is backslash ('\', ascii code 92)
   ch is hyphen ('-', ascii code 45) AND the following character is a digit

acceptance:
  after skipping leading junk,
  keep the char ch if any of these are true:
   ch is a letter or a digit
   ch is backslash  and next char is a letter or digit
   ch is apostrophe and next char is a letter or digit
   ch is hyphen     and next char is a letter or digit
   ch is period     and next char is a letter or digit
   ch is comma      and is surrounded by digits
   ch is colon      and is surrounded by digits
   ch is the first or second of a pair of colons, the pair surrounded by letters
  or digits

s.Element(i) returns s[i] by value if i < s.Size() and returns '\0' otherwise.

*/

#include <iostream>
#include <wordbench2.h>

bool isLetter(const char& c)
{
  if((c > 64 && c < 91) || (c > 96 && c < 123))
  {
    return true;
  }
  else 
  {
    return false;
  }
}

bool isNumeric(const char& c)
{
  if(c > 47 && c < 58)
  {
    return true;
  }
  else 
  {
    return false;
  }
}

bool isLetterOrDigit(const char& c)
{
  if(isLetter(c) || isNumeric(c))
  {
    return true;
  }
  else
  {
    return false;
  }
}

char toLowerCase(const char& c)
{
  if(c > 64 && c < 91)
  {
    return c+32;
  }
  else 
  {
    return c;
  } 
}
 

void WordBench::Wordify(fsu::String& s)
{
  // itr is a string's element counter
  // start is the element number where the word begins
  size_t itr = 0, start = 0, stop = 0;
  bool valid = false;
  //Finding the leading character for the word
  while(s.Element(itr) != '\0' && !valid)
  {
    char c = s.Element(itr);
    if (c == '-' && isNumeric(s.Element(itr+1)))
    {
      start = itr;
      valid = true;
    }
    else if (c == '\\')
    {
      start = itr;
      valid = true;
    }
    else if (isNumeric(c))
    {
      start = itr;
      valid = true;
    }
    else if(isLetter(c))
    {
      start = itr;
      s[itr] = toLowerCase(s[itr]);
      valid = true;
    }
    ++itr;
  } //while

  if(valid)
    s[0] = s.Element(start);
  //structuring word to be placed in the set

  size_t it = 1;
  while(valid)
  {
    //if(itr == start) //print first letter (for testing)
    //  std::cout << s[itr] << std::endl;
    char c = s.Element(itr);
    if (isLetterOrDigit(c))
    {
      if(isLetter(c))
      {
        c = toLowerCase(c);
      }
      s[it] = c;
    }
    else if (c == '-')
    {
      if(isLetter(s.Element(itr-1)) && isLetter(s.Element(itr+1)))
      {
        s[it] = c;
      }
      else if(isNumeric(s.Element(itr+1)))
      {
        s[it] = c;
      }
      else
      {
        valid = false;
        stop = it;
      }
    }
    else if(c == '.' && isLetterOrDigit(s.Element(itr-1)) && isLetterOrDigit(s.Element(itr+1)))
    {
      s[it] = c;
    }
    else if (c == '\\' && isLetterOrDigit(s.Element(itr + 1)))
    {
      s[it] = c;
    }
    else if (c == ',' && isNumeric(s.Element(itr - 1)) && isNumeric(s.Element(itr + 1)))
    {
      s[it] = c;
    }
    else if ((c == ':' && s.Element(itr + 1) == ':') && isLetterOrDigit(s.Element(itr-1)) && isLetterOrDigit(s.Element(itr+2)))
    {
      s[it] = c;
    }
    else if ((c == ':' && s.Element(itr - 1) == ':') && isLetterOrDigit(s.Element(itr+1)) && isLetterOrDigit(s.Element(itr-2)))
    {
      s[it] = c;
    }
    else if (c == ':' && isNumeric(s.Element(itr - 1)) && isNumeric(s.Element(itr + 1)))
    {
      s[it] = c;
    }
    else if (c == '\'' && isLetter(s.Element(itr+1)))
    {
      s[it] = c;
      //std::cout << "Apostrophe" << std::endl;
    }
    else if (valid)  // when invalid character is found
    {
      if(isLetterOrDigit(s.Element(itr-1)))
        stop = it; //one after the last valid element which must be a char/ number
      valid = false;
    }
    ++it;
    ++itr;
    
  }// while

 
  //if(valid == false)
  //{
    s.SetSize(stop);
    //    std::cout << "Cleaned word: " << s << std::endl;
    //}
} // Wordify()
