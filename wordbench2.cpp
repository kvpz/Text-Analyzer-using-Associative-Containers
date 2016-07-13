/*
  wordbench2.cpp
  Kevin Perez
  06/13/15

  This implementation file of Wordbench. The Set API is used which provides
  logarithmic time for insertion, deletion, and searching. 
  The procedure ReadText extracts string
  from a file; each string is "cleaned up" i.e. extract a word from the
  string. The criteria for what qualifies as a word can be found in wordify.cpp, the
  "cleaner-upper." 
  WriteReport() writes to a file specified by the user all the words and how
  many times they appeared in the text.

UPDATE:
7/19/15
  This is a refactored version of wordbench.cpp that was used an Ordered Set of
  Pairs API. That version of Wordbench can be found in COP4530/proj4. The
  development of this refactored class was tested and compared with the main client
  program as in proj4; plus I will be testing with moaa.cpp/ foaa.cpp/ foaa+.cpp
  found in proj5 for functionality tests.


 */

#include <wordbench2.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "wordify.cpp"

WordBench::WordBench() : count_(0)
{
}
  
WordBench::~WordBench()
{
  std::cout << "In WordBench destructor" << std::endl;
}

bool WordBench::ReadText(const fsu::String& infile)
{
  std::ifstream fstr;
  fstr.open(infile.Cstr());
  if(fstr.fail())
  {
    return false;
  }
  else
  {
    fsu::String str;
    infiles_.PushBack(infile);
    unsigned int wordsRead = 0;
    return true;
  }//outer-if
}

bool WordBench::WriteReport(const fsu::String& outfile, unsigned short kw, unsigned short dw,
                std::ios_base::fmtflags kf, std::ios_base::fmtflags df ) const
{
 if (infiles_.Empty())
 {
   std::cout << "  No files in read list, leaving " << outfile << " unopended" << std::endl;
 }
 else
 {
  std::ofstream ostr;
  ostr.open(outfile.Cstr());
  if(ostr.fail())
  {
    return false;
  }

   ostr.close();
   return true;
  }
  return true;
}

void WordBench::ShowSummary() const
{

}

void WordBench::ClearData()
{
  frequency_.Clear();
  count_ = 0;
  infiles_.Clear();
  std::cout << "\tCurrent data erased" << std::endl;
}

/*
void WordBench::Wordify  (fsu::String& s)
{

}
 */