#!/bin/sh

projname=$1

# mkdir $projname

echo "Creating project: $projname"

cp -R ../ProjectTemplateSDL ../$projname

sed -i "" "s/templateApp/$projname/g" ../$projname/makefile

cprojname="TemplateProgram"

sed -i "" "s/$cprojname/$projname/g" ../$projname/src/main.c


cd ../$projname

make;make run; make clean