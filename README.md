# CHL
C Hypertext Library - A library for writing web applications in C

## What is CHL?

CHL, short for *C Hypertext Library*, is a library written in C to be used to write web applications of all sorts. CHL provides a lot of useful features:

> - *Set, configure and remove cookies and sessions.*
- *File management: upload and store files on the server.*
- *Inline functions and variables in Hypertext documents using the '<{ function(variable); }>' syntax.*
- *Database management using the Mysql API.*
- *Functions for handling of POST and GET data.*
- *Set and change various HTTP headers.*

and a lot more...

## How does it work?

Web applications written in C must somehow be executed by the Web server since C is a compiled langauge. To make this possible CHL is based on CGI (Common Gateway Interface), which in short means that the server executes your program and passes it data through environment variables. CHL does all the parsing and handling of CGI in the background, and provides you with some easy to use functions instead.

## Setup

For a proper guide on how to install and configure CHL on your system, read the following document: https://github.com/it4e/CHL/blob/master/SETUP.md

As of now CHL is only officially available on Linux, but I am planning to port it to other operating systems as well if the interest is big enough. Otherwise you could of course make a port of it yourself.
