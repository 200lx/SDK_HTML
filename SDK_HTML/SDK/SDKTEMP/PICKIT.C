/*
5 November 1998
PICKIT v1.0
For use with the HTML version of the 200LX SDK

This program is partially a PAL program which displays a 
dialog box prompting for search input, and partially a
regular DOS program which produces necessary output files
for the search to be complete.
*/

// Standard include files

#include <stdio.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <process.h>
#include <io.h>

// PAL's special header for those nifty functions
#include "pal.h" 

// Constants for use later
#define CBSELECT 0
#define BNOK 2     // value for OK button
#define BNCANCEL 3 // value for CANCEL button

// header file created by PDE
#include "resource.h"

/*[PDE:Search]*/

// PDE-created structure for chapter select list
IHCBINIT CBCHAPTERS = {
{"All",  20, MEDIUM_FONT },
"01|02|03|04|05|06|07|08|09|10|11|12|13|14|15|16|All"
};

// PDE-created structure for OK button
IHBNINIT BNOKBUTTON = {
MEDIUM_FONT
};

// PDE-created structure for entire Window
DLGITEM SearchItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{  4,  5,544, 14,   0L, "&Find:",    NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{  3, 25,222, 14,   0L, "&In Chapters:", &CBCHAPTERS,IhCombo , sizeof(IHCOMBO ), NULL },
{259, 52, 50, 25, IF_RET, "&Go!",      &BNOKBUTTON,IhButton, sizeof(IHBUTTON), NULL },
{329, 52, 80, 25, IF_ESC, "&Cancel",   NULL,       IhButton, sizeof(IHBUTTON), NULL }
};

// PDE-created structure for the whole dialog box
DIALOG Search = {
554, 98,
0L,
0,
4,
SearchItems,
DhStandard,
NULL,
NULL,
0
};


int main(void)
{
// file handle integers and a byte count integer
int fileNum,outFile,someBytes;

// counter variable
int x=0;

// too many string arrays to detail
// but basically just buffers and strings for text
char Buffer[200];
char Buffer2[200];
char finBuff[200];
char tmpBuff[3];
char buffTemp[200];
char bufferit[200];
char elBuffero[10000];
char theBuffer[10000];

// I'll use this to figure out what button the user pressed
int result;

// various "flag" variables
int quitIt=0;
int tmpInt=0;
int lineCount=0;

// initializing tmpBuff (it's small)
tmpBuff[0]=0;
tmpBuff[1]=0;
tmpBuff[2]=0;

// initializing the largest arrays
for(x=0;x<10000;x++)
{
 theBuffer[x]=0;
 elBuffero[x]=0;
}

// initializing the medium arrays
for(x=0;x<200;x++)
{
 Buffer[x]=0;
 Buffer2[x]=0;
 finBuff[x]=0;
 buffTemp[x]=0;
 bufferit[x]=0;
};

// open the file "grepit.bat" for output only
fileNum=open("grepit.bat",O_WRONLY | O_CREAT,0666);

if(!PalInit(1)) exit(-1); /* Initialize PAL */
InitDialog(&Search);   /* init dialog */

// Show the dialog with the title listed
ShowDialog(&Search, 10, 10, "Search the Developer's Guide");

// Get keystrokes, store exit code in result variable
result=HandleDialog(&Search, NULL);

// get text to find, put it in Buffer
GetDlgItem(&Search, CBSELECT, EDGI_TXT, Buffer);

// get chapters to search, put that text in Buffer2
GetDlgItem(&Search, 1, EDGI_TXT, Buffer2);

// Dialog is over
CloseDialog(&Search);     /* close dialog */

// quit Pal, exit graphics mode, etc.
PalDeInit(1);

// Do all this if the user chose OK, and not CANCEL
if(result==BNOK)
{
 // if chapters to search was default of "All"
 if(!strcmp(Buffer2,"All"))
 { 
   // Store "*.htm" in finBuff
   finBuff[0]='*';
   finBuff[1]='.';
   finBuff[2]='h';
   finBuff[3]='t';
   finBuff[4]='m';
   finBuff[5]=0;
  }
 else //otherwise, search specific chapters
 {
  // repeat until the end of our inout for "chapters"
  for(x=0;x<strlen(Buffer2);x++)
  {
   tmpBuff[0]=Buffer2[x];  // get one letter
   tmpBuff[1]=Buffer2[x+1]; // get next letter
   tmpInt=atoi(tmpBuff); // convert that value to integer

   // depending on value of the integer, we'll
   // append those chapter titles to the list of
   // files to search with Xgrep

   if(tmpInt==1)
    strcat(finBuff," C1*.htm");
   if(tmpInt==2)
    strcat(finBuff," C2*.htm");
   if(tmpInt==3)
    strcat(finBuff," C3*.htm");
   if(tmpInt==4)
    strcat(finBuff," C4*.htm");
   if(tmpInt==5)
    strcat(finBuff," C5*.htm");
   if(tmpInt==6)
    strcat(finBuff," C6*.htm");
   if(tmpInt==7)
    strcat(finBuff," C7*.htm");
   if(tmpInt==8)
    strcat(finBuff," C8*.htm");
   if(tmpInt==9)
    strcat(finBuff," C9*.htm");
   if(tmpInt==10)
    strcat(finBuff," 10C*.htm");
   if(tmpInt==11)
    strcat(finBuff," 11C*.htm");
   if(tmpInt==12)
    strcat(finBuff," 12C*.htm");
   if(tmpInt==13)
    strcat(finBuff," 13C*.htm");
   if(tmpInt==14)
    strcat(finBuff," 14C*.htm");
   if(tmpInt==15)
    strcat(finBuff," 15C*.htm");
   if(tmpInt==16)
    strcat(finBuff," 16C*.htm");
   // increment the position variable by one
   // extra, so we jump by 2 each time
   // (or else it would read 16 as 16, then 6)
   x++;
  } // done looping through chapters
 }  // done creating strings to search chapters

 // here we will write the batch file that we will later
 // execute.  someBytes will contain the number of bytes
 // written each time.
 
 someBytes=write(fileNum,"@echo off\n",10);

 // note: \" inside quotes means output a " character
 someBytes=write(fileNum,"xgrep -lsy \"",12);
 someBytes=write(fileNum,Buffer,strlen(Buffer));
 someBytes=write(fileNum,"\" ",2);
 someBytes=write(fileNum,finBuff,strlen(finBuff));
 someBytes=write(fileNum," > grepall.lst",14);
 close(fileNum); // done with the batch file
 // inform the user about what is going on...
 printf("Searching selected files...\n");
 
 // execute the batch file
 system("grepit.bat");
 
 // open the batch file's output file
 // containing a list of all file that had the
 // search phrase in them.  Format will be
 // something like:
 // 
 // C8000006.HTM
 // C8000007.HTM
 // C8000008.HTM
 // etc.  One file name per line.
 fileNum=open("grepall.lst",O_RDONLY,0666);
 
 // open the HTML file we will create.
 outFile=open("s.htm",O_WRONLY | O_CREAT,0666);

 x=0; // initialize counter again

 while(!eof(fileNum)) // until we reach the end of the file
 {

  // read in 13 bytes of file 
  // (eleven characters total for name,
  // plus period makes 12, plus end-of-line
  // character makes 13
  someBytes=read(fileNum,buffTemp,13);
  
  // someBytes will equal 13 if the read was
  // successful.  If not, that means we have
  // hit the end of the file and did not
  // get a real filename in.
  if(someBytes!=13)
  {
   quitIt=1; // set the quitIt flag so we know to stop
  }

  // execute everything in the "if" statement only if
  // we obtained a valid filename in buffTemp
  if(!quitIt)
  {
   for(x=0;x<200;x++) // reinitialize
    bufferit[x]=0;    // the bufferit array
    
   // storing 0 in position 12 of the buffTemp
   // array terminates the string properly
   buffTemp[someBytes-1]=0;
   
   // append all this text to the bufferit string
   // which will be appended to elBuffero
   // which will eventually contain the full text
   // of the HTML file to be output, except
   // for header and body info.
   // note that \" inside quotes means " character

   // begin A HREF tag
   strcat(bufferit,"<H2><A HREF=\"");

   // append filename to A HREF tag
   strcat(bufferit,buffTemp);
   
   // finish off A HREF tag
   strcat(bufferit,"\">");

   // convert buffTemp to lower case
   // for display in the search results page
   strlwr(buffTemp);

   // append filename (actual part that will
   // display in HTML page) to bufferit
   strcat(bufferit,buffTemp);
   
   // completely end this line
   strcat(bufferit,"</A></H2>\n");

   // add this to elBuffero, which will
   // be the full text output to the
   // HTML file when done.
   strcat(elBuffero,bufferit);

   // add 1 to number of files found
   lineCount++;

  } // done with if statement, go do WHILE again
 } // done with WHILE loop
 
 // having created the "meat" of the output HTML file
 // in elBuffero, we'll now copy it into
 // theBuffer which will first have the rest of the HTML
 // document code in it.
 // theBuffer will then contain the full text of the output
 // file.

 // put initial lines into HTML code
 strcat(theBuffer,"<HTML><HEAD><TITLE>Search results</TITLE>\n");

 // convert lineCount to string for output (base 10)
 itoa(lineCount,bufferit,10);
 
 // more HTML code
 strcat(theBuffer,"<BODY><BR><H1>");

 // output the string with lineCount's value in it
 strcat(theBuffer,bufferit);
 
 // use that to display number of files found
 strcat(theBuffer," files found</H1><BR><HR><BR>\n<UL>\n");

 // output the main part of the file
 // containing search results
 strcat(theBuffer,elBuffero);
 
 // and then end the file
 strcat(theBuffer,"</BODY></HTML>");

 // dump the contents of theBuffer to outfile
 someBytes=write(outFile,theBuffer,strlen(theBuffer));
 
 close(fileNum); // done with input file
 close(outFile); // done with output fiele 

 // exit the program and
 // return ERRORLEVEL 128
 // indicating success and that the person chose
 // to do a search.
 return(128);
 } // end if statement (if person chose "OK")

 // if person chose CANCEL (hit "search" accidentally
 // for example) then we don't want to go through all
 // that rigamarole above.
 
 printf("Search terminated, returning to Guide...\n");

 // exit and return ERRORLEVEL 1
 // indicating search was canceled
 return(1);

} // end of program
