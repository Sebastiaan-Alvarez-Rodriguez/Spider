# Spider the webspider
The webspider, webquery, and picture compare programs are in this source directory.
To get this thing working, perform the following steps (in specified order):
 1. Type `make fast` (produces 3 executables)
 2. Type `./webspider https://www.universiteitleiden.nl 300` to crawl the web, download pictures and create reverse indexes.
 3. Type `python3 server.py` to start the server
 4. Open your browser, and go to localhost:8080

Now, you should see our search engine UI. Just type a keyword such as `Leiden`, `CoRoNaViRuS`, `Afrika`, or some other keywords found on Leiden university website urls and titles.
Also, check out the reverse image search window.
