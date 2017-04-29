# [MyCurl](https://github.com/GhostWriterTNCS/MyCurl)

A small collection of functions used in my projects working with [cURL](https://curl.haxx.se/).

In order to use this functions, remember to include the static library **libcurl_a.lib** and to add the compile command **-DCURL_STATICLIB**.

To obtain the library, I followed this method (from [this post](http://stackoverflow.com/questions/20171165/getting-libcurl-to-work-with-visual-studio-2013/28641974#28641974) on Stack Overflow):

* Add Visual Studio to the system PATH:  
	For example: *C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin*.
* Download current curl-X.zip from http://curl.haxx.se/download.html and unzip.
* Open command line at *curl-X/winbuild*.
* Call *vcvars32.bat*.
* Call *nmake /f Makefile.vc mode=static VC=12* (change number to fit your version of Visual Studio).
* Go to *curl-X/builds/libcurl-XXX*. There you'll find the includes and the libcurl_a.lib.

You can use *MACHINE=x64* with nmake to build an x64 lib.
