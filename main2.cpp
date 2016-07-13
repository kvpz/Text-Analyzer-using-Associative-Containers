/*
     main2.cpp
     02/16/25

     driver program for WordBench2
*/

#include <wordbench2.h>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <fstream>

void  DisplayMenu ();

int main( int argc, char* argv[] )
{
  bool BATCH = 0;
  if (argc > 1) BATCH = 1;
  std::istream* isptr = &std::cin;
  std::ifstream ifs;
  if (BATCH)
  {
    ifs.open(argv[1]);
    if (ifs.fail())
    {
      std::cout << " ** Error: cannot open file " << argv[1] << '\n';
      return 0;
    }
    isptr = &ifs;
  }
  std::cout << "Welcome to WordBench, a Text Analysis Utility.\n";
  if (!BATCH) DisplayMenu();

  WordBench wb;       // declaring WordBench2 object
  char selection;  
  fsu::String filename;  

  do
  {
    std::cout <<   "\nWB command ('m' for menu, 'q' to exit): ";
    *isptr >> selection;
    if (BATCH) std::cout << selection << '\n';
    switch (selection)
    {
      case 'r': case 'R':
        std::cout << "  Enter file name : ";
        *isptr >> filename;
        if (BATCH) std::cout << filename << '\n';
        while (!wb.ReadText(filename))
        /* ReadText returns TRUE when all of the document has been processed */
				{
          std::cout << "    ** Cannot open file " << filename << '\n'
                    << "    Try another file name: ";
          *isptr >> filename;
          if (BATCH) std::cout << filename << '\n';
        }
        break;
       
      case 'w': case 'W': 
        std::cout << "  Enter file name: ";
        *isptr >> filename;
        if (BATCH) std::cout << filename << '\n';
        while (!wb.WriteReport(filename))
        {
          std::cout << "    ** Cannot open file " << filename << '\n'
                    << "    Try another file name: ";
          *isptr >> filename;
          if (BATCH) std::cout << filename << '\n';
        }
        break;

      case 'c': case 'C':
        wb.ClearData();
        std::cout << "\n     Current data erased\n";
        break;

      case 's': case 'S':
        wb.ShowSummary();
        break;
     
      case 'm': case 'M':
        DisplayMenu();
        break;

      case 'x': case 'X':
        if (BATCH)
        {
          std::cout << "  switching to interactive mode\n";
          isptr = &std::cin;
          ifs.close();
          BATCH = 0;
          DisplayMenu();
        }
        else
        {
          std::cout << "  already in interactive mode\n";
        }
        break;
     
      case 'q':  case 'Q':
        if (BATCH)
          ifs.close();
        selection = 'q';
        break;
     
      default:
        std::cout << "\n     ** Command not found -- try again\n";
    }
  }
  while (selection != 'q');

  std::cout << "\nWordBench wishes you a nice day." << std::endl;
  return EXIT_SUCCESS;
}  // end main()

void DisplayMenu()
{
  std::cout << '\n'
            << "     WB Command                     key\n"
            << "     ----------                     ---\n"
            << "     Read a file  ................  'r'\n"
            << "     show Summary  ...............  's'\n"
            << "     Write report  ...............  'w'\n"
            << "     Clear current data  .........  'c'\n"
            << "     eXit BATCH mode  ............  'x'\n"
            << "     display Menu  ...............  'm'\n"
            << "     Quit program  ...............  'q'\n";
}
