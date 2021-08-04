#!/bin/sh
echo "Content-type: text/html"
echo ""
echo "<html>"
echo "<head><title>Hello HTTP/XRCE</title></head>"
echo "<body>"
echo "<h1>Input Data</h1>"
echo "<pre>"
cat -
echo
echo "</pre>"
echo "<h1>CGI Environment</h1>"
echo "<pre>"
env
echo "</pre>"
echo "</body>"
echo "</html>"
