# AoC

Dump of all my solutions for the advent of code 2024. All written in C++ with simple make file and run script to support development. Some solution are clean, other are a mess :-). Use it at your own risk.

## Structure

Each day has two folders. Example 04 of dec has 04a and 04b. 04a is the solution to the first part. I basically just copies the solution when I find the first star to 04b. Sometime I forget to switch the open file and therefore I may have continued on the second part in the first folder without realizing it.

The run.sh script is used to build and send all the *.input files as stdin to the files. I later abandoned this strategy and started to open the file from within the code. It made debugging a bit easier but then I had to update the code to go from the example to full input.

I have a set of "default" includes that may be handy. When starting a new day I just copy the previous solution (or one that had some structures that could be reused). This year was a lot of 2d map puzzles + some priority queue ones.

## Lessons learned

It is mhy 4th year doing advent of code and it is a lot of fun. This year worked out rather smooth but I struggled a bit on the later ones like day 21. I think the most fun puzzle was day 14 (second part) where you compare solutions to others the solution may be very different.

Also this years strategy to do TDD development payed out well. With some extra scoping + assert I manage to smoke out most of the parsing errors and verify that code was on track with the instructions given by each puzzle.

Next your I really need to:
* Learn to get up early to get better points. I'm not a morning person and prefer to do the solution during the evening. But since puzzles are released at 06:00 sweden time creating answers at 19:00 will not give any good score.
* I really need to find a better way to parse the input. For most puzzles over half of the time is spent parsing the input.
* Replace the simple makefile with something a bit more flexible.

/Alexander
