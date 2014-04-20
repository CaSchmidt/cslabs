cslabs - The Software Laboratory of Carsten Schmidt
===================================================

About
-----

This repository countains some of the code I am working on.
As you will find out, most of it is written in C++ utilizing Qt as the main dependency.

License
-------

As indicated in the various source files, the code is licensed under a 3-clause BSD license.

Contents
--------

cslibs (base libraries):

* csQt: Some classes frequently needed but sorely missing in the official Qt library.
* csPDF: A PDF rendering library wrapping [MuPDF](http://www.mupdf.com/) and targeting Qt's QImage.

csapps:

* CS::PDF: An intentionally minimalistic PDF viewer. My testbench for the above mentioned csPDF library.
