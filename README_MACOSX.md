1. Prepare your system
~~~
macosx% curl https://raw.githubusercontent.com/dchassin/gridlabd/dchassin/master/requirements.sh | bash
~~~

2. Download the source
~~~
macosx% cd /usr/local/src
macosx% git clone https://github.com/dchassin/gridlabd gridlabd
~~~

3. Build gridlabd
~~~
macosx% autoreconf -isf
macosx% ./configure
macosx% make install
~~~

4. Validate the installation
~~~
macosx% gridlabd --validate
~~~

