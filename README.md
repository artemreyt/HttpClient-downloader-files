# HttpClient-downloader-files
C++ HttpClient which downloads and saves files by URL.

### Installing
To build a programm, make:
```
mkdir build
cd build
cmake ..
make
```
If all is ok, in directory build will be binary "main".
### Usage
Do  ```./main url```, where ```url``` is URL of file to download.
Now, it's only one protocol is available - HTTP, no HTTPS. Examples of http URLS you can find in file *refs.txt* in the root of this repo.
### Example
```
$ ./main "http://www.chilkatsoft.com/hamlet.xml"
Connection established
File downloaded and written in hamlet.xml
$
$
$ head -10 hamlet.xml 
<?xml version="1.0"?>

<PLAY>
<TITLE AUTHOR="William Shakespeare">The Tragedy of Hamlet, Prince of Denmark</TITLE>

<FM>
<P>Text placed in the public domain by Moby Lexical Tools, 1992.</P>
<P>SGML markup by Jon Bosak, 1992-1994.</P>
<P>XML version by Jon Bosak, 1996-1998.</P>
<P>This work may be freely copied and distributed worldwide.</P>
$
```
