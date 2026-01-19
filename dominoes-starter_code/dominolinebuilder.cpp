#include "dominolinebuilder.h"

Domino::Domino(std::string theBlueSymbol, std::string theRedSymbol)
{
   blueSymbol = theBlueSymbol;
   redSymbol = theRedSymbol;
}

DominoLineBuilder::DominoLineBuilder(unsigned long int totalNumberOfDominoes, std::istream& dominoInputData)
{
   for (unsigned long int i = 0; i < totalNumberOfDominoes; ++i)
   {
       std::string aBlueSymbol, aRedSymbol;
       std::getline(dominoInputData, aBlueSymbol, ':');
       std::getline(dominoInputData, aRedSymbol, '\n');

       disorderedDominoes.push_back(Domino(aBlueSymbol,aRedSymbol));
   }
}

bool DominoLineBuilder::nextRight()
{
   if (orderedLine.empty())
   {
      orderedLine.push_back(disorderedDominoes.back());
      disorderedDominoes.pop_back();
      return true;
   }

   for (Domino currentDomino : disorderedDominoes)
   {
      if (currentDomino.blueSymbol == orderedLine.back().redSymbol)
      {
         orderedLine.push_back(currentDomino);
         return true;
      }
   }

   return false;
}

void DominoLineBuilder::displayLine(std::ostream& outputStream)
{
   for (Domino eachDomino : orderedLine)
   {
      outputStream << eachDomino.blueSymbol << ':' << eachDomino.redSymbol << ' ';
   }
}
