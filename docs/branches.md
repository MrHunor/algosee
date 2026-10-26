# Branches  
As of writing this there are currently three branches:  

## main  
Main ALWAYS presents as a working version without any obvious bugs, while some misimplementations (e.g. some algos being presented by the frontend but not implemented by the backend ) may be present.  
It is important that the code on main is bug free because the render server mirrors the newest version of main, same for the github pages.  
So you could say main is the **production** branch.

## development 
Development presents as the branch that is used for, you guessed it, development. New features or bugfixes are availabe here first, with not guarentee that this version will work as intended or even compile. On this branch, you might see commits like `won't build` or `doesn't work`, that's normal. You might think of creating a new branch for each new feature or bugfix, but there are two reasons not to do this, firstly this project is maintained by two people who each have their own part of the code, we rarely ever interfere, and more imporantly i (@MrHunor) can confidently say that i have no idea how to properly resolve merge conflicts.

## tempDevelopment
This is, again you guessed it, as temporary branch, currently it is used to host commits that are not made to development and will later be pulled onto development.  
The reason for this is simple, currently there is a PR on development to main, now if i were to push new commits that would (likely) generate new problems with the front to backend communcation, and since there are already some problems i dont want to overload @siryanni. When he fixes the frontend issues we will pull this branch onto development as said before.  

# For Contributers 
If you're looking to contribute, we could, in contrast to the information given in the development section, ask you to create a fork of the repo and implement your changes there. :)