# mBot Line Follower - Ben, Cristina, Jerry
This is the commit of my line follower maze code. The speed parameter and conditionals are tuned for 3/4" tape and 2 double light sensors. The code has been placed into separate functions and are called accordingly from the main loop. Note the count of left turns and right tunrs; this in theory is supposed to reduce the mBot's chances of getting stuck in a cycle:
```
if(right.readSensors()==2 && rturns<=3){
      rightturn();
}
```
Intersections are the most ambiguous, and therefore require us to check if we are on the goal pad, going straight, or turning. We do this by driving forward slightly, and then backing up and turning if we are at a 3 way intersection or going straight if we are at a 4 way intersection:
```
if(left.readSensors()==2 || right.readSensors()==1){
  straight();
}else{
  backup(); 
  delay(2000);
  if(lturns>=3){
    rightturn();
  }
  if(rturns>=3){
    leftturn();
  }
}
```
Notice too the adjright() and adjleft() methods; these keep the mBot on track if it begins to move off of the tape by increasing one motor for a short period of time until me are completely back on the line.

Together these conditions word together in the main loop to make to maze follow a line with a variety of complexity.
