/*
  wordbench2.h
  6/13/15
  Kevin Perez
  
  OAA behaves 
  like a class with one template parameter because P is default to 
  LessThan<T>, but it can be intialized with another predicate class otherwise. 
  
  Wordify is a helper method (included as a slave file) written in wordify.cpp
  that is used to cleanup the string passed to it by reference. 

*/

#ifndef WORDBENCH_H
#define WORDBENCH_H
#include "xstring.h"
#include <list.h>
#include <oaa.h>


class WordBench
{
public:
  WordBench         ();
  virtual ~WordBench(); 
  bool ReadText     (const fsu::String& infile);
  bool WriteReport  (const fsu::String& outfile, unsigned short kw = 15, unsigned short dw = 15,
                     std::ios_base::fmtflags kf = std::ios_base::left, // key justify
                     std::ios_base::fmtflags df = std::ios_base::right // data justify
                     ) const; 
  void ShowSummary  () const;
  void ClearData    ();

private:
  typedef fsu::String             KeyType;
  typedef size_t                  DataType;

  size_t                          count_;  //number of valid words read
  fsu::OAA  < KeyType, DataType > frequency_;
  fsu::List < fsu::String >       infiles_;
  static void Wordify  (fsu::String&);
};
//#include <wordify.cpp>
#endif
