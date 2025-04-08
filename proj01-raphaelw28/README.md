# Project 1 - The Warehouse Project

## Project Description

For this project, you will complete a program that simulates working at the warehouse in a kit assembly company.

![warehouse](https://github.com/CS220-2024S/Images/blob/main/warehouse.jpg "Warehouse Image")


Your job is to assemble a kit that consists of various parts that are stored in the warehouse. There are 1,000 different parts stored in the warehouse, identified by a part number from 0 to 999. The parts are stored in bins; each bin contains an unlimited supply for ten different sequential part numbers.  So part numbers 0-9 are stored in bin 0, part numbers 10-19 are stored in bin 1, and so on up to bin 99 which has part numbers 990 to 999.

Each kit has an order list that lists all of the parts that belong in the kit. Your job is to get the parts on the order list from the bins they are kept in, and put them into the kit. Your work station at the warehouse has a workbench that has slots for five different bins, but you need to fetch those bins from the warehouse to a slot on your workbench before you can get the parts out of the bin and into your kit.

![birdhouseKit](https://github.com/CS220-2024S/Images/blob/main/birdhouse_kit.jpg "Birdhouse Kit Image")

For example, a bird house kit might have an order list as follows:

| Part | Description | Part | Description |
|----|----|----|-----|
| 472 | Base | 476 | Right Wall |
| 473 | Roof | 306 | 5/8" screw |
| 214 | Sand Paper | 306 | 5/8" screw |
| 482 | Front Top Brace | 306 | 5/8" screw |
| 483 | Back Top Brace | 306 | 5/8" screw |
| 324 | Hinge | 346 | Hanging Hook |
| 324 | Hinge | 346 | Hanging Hook |
| 304 | 1/2" screw | 487 | Roost Dowel |
| 304 | 1/2" screw | 216 | Paint (Red, Yellow, Blue) |
| 474 | Back Wall | 219 | Paint Brush |
| 475 | Front Wall | 349 | Rope |
| 476 | Left Wall | 137 | Assembly Instructions |

In order to assemble this kit, you would first need to fetch bin 47 from the warehouse to a slot on your workbench. You could then get parts 472 and 473 from bin 47 to put into you kit. Then you would need to fetch bin 21 so you could get part 214, 48 for part 483, 32 for the two hinges with part number 324, and 30 for the 304 screws. At this point all the slots on your workbench are full.  Here's what your workbench would look like at this point:

| Bin  | 47  | 21  | 48  | 32  | 30 |
|------|-----|-----|-----|-----|-----|
| Slot | 0   | 1   |   2 |   3 | 4 |

Since you already have bin 47 on your workbench you can still get parts 474, 475, and 476. You also have bin 30, so you can still do part 306. However, at this point you need bin 34 for part number 346, but there is no free slot on your workbench.

At this point, you need to decide which bin to send back to the warehouse so you can free up a slot to add bin 34. If you could look ahead in the order, you could see that you will still need bin 21 and 48 later on, so your shouldn't send those back. The problem is, in this warehouse, you aren't allowed to see anything beyond the current part number you are working on, so there is no opportunity to look ahead.

Just to finish up this scenario, suppose you choose to send bin 47 back to the warehouse, and put bin 34 in its place. That gets you up to part 137, so now you send bin 21 back to the warehouse and get bin 13 into slot 1.

Now you have finished the order. The kit contains 24 parts, but you needed to fetch 7 different bins from the warehouse, and return 2 bins in order to complete the kit.

This warehouse pays you based on how much profit we earn from each kit you put together. Since the average of the sales price of each part is 10 cents, the net sales price of the kit is the number of parts times 0.10. The warehouse changes 10 cents to fetch a bin from the warehouse, and another 5 cents to return a bin from your workbench to the warehouse. Therefore, in the scenario above, the net sales price for the 24 items in the bird house kit is $2.40, but the cost was `7*0.10 + 2*0.05 = $0.80`, so the net profit was $1.60.

So the challenge for this assignment is to figure out how to manage your workbench so that you can earn the highest possible profit. That means you need to somehow predict which bins on the workbench are the *least* likely to be needed again so you can send them back. However, since you can't look ahead in the order, you need to be able to figure out which bins to send back based on what is in the order that you have already processed. Here is the big hint for this assignment:

**The part numbers in an order list tend to be sequential, but when the sequence is *not* followed, there is a high probability that the next part number showed up recently in the order.**

## Warehouse Simulation Infrastructure

You have been provided with the basic infrastructure for the C code to simulation the warehouse described above. The initial repository contains the following files:

- **warehouse.c** - C source code that contains a main function to simulate the warehouse, along with some utility functions that you can invoke to make the project easier. Each of these functions is described below. You are not allowed to modify the warehouse.c code for this project. Even if you do modify the code, we will use the original warehouse.c when grading your code.
  - `main` function - This function treats the command line arguments to the warehouse program as the names of text files which contain order lists - lists of part numbers. For each command line argument, the main function resets all the statistics for the warehouse, and then reads each part number. The main function invokes the `processPart` function for each part number on the list to give you an opportunity to manipulate the bins on your workbench. Once `processPart` is complete, the main function checks to make sure the part is available in a bin on the workbench. If not, the program stops. As long as the part is available, the main function continues processing parts, invoking processPart for each part, until the order is complete. Then, main prints out the sales prices and net gain for that order.
  - `binFromPart` function - This function returns the bin number for the bin that contains the part number specified as an argument.
  - `binInSlot` function - This function returns the bin number of the bin in the slot on your workbench specified as an argument (a number between 0 and 4).
  - `onBench` function - This function returns a 1 if the bin specified as an argument is in a slot on your workbench, or a 0 if the bin is not on your workbench.
  - `getBin` function - This function sends the bin in the slot specified as the second argument back to the warehouse if there is a bin in that slot, and then fetches the bin specified as the first argument from the warehouse and puts in on your workbench at the slot specified as the second argument.
  - `partIndex` function - This function returns the index of the current part number in the order. The first part has index 0, the second index 1, and so on.
  - `swapSlots` function - This function allows you (at no cost) to swap two bins on your workbench. You specify the two slots to swap bins in as arguments. (You probably won't need this, but it's there just in case you want it.)

- **warehouse.h** - The C include file that contains the function definitions for the functions in warehouse.c that are available for you to use. You should not modify this file.

- **packer.h** - The C include file that describes the processPart function that needs to be defined in the packer.c file. You should not modify this file.

- **packer.c** - This is the only C file you need to modify. The only requirement for this file is that it contains the definition of the `processPart` function. The `processPart` function needs to manipulate your workbench to make sure that the part specified in the argument is in a bin that is on the workbench when the program returns.
  
  The code that is in this file as it is delivered uses the simplest (and least effective) method for managing the workbench. The "dumb" implementation uses only slot 0 on the workbench. If the bin for the part number is not already in slot 0, then the code sends that bin back to the warehouse, and gets the required bin and puts it in slot 0. Your job is to replace the code in this function with a more effective algorithm that results in greater savings.

- **Makefile** - A make file that contains several targets, as follows:
  - **test** - A pseudo-target to invoke the warehouse executable file for order1.txt, order2.txt, and order3.txt.
  - **warehouse** - Builds the warehouse executable file using the code in both warehouse.c and packer.c.
  - **warehouse_debug** - Builds the warehouse executable file with a special debug flag that causes the state of the bins on the workbench to get printed every time they are changed. Use `make clean` to remove the debug version and revert to the non-debug version of the warehouse executable.
  - **clean** - Removes the warehouse executable.

- **order1.txt**, **order2.txt**, and **order3.txt** - Three example order files that can be used for testing.

## Working the Project

The infrastructure, as delivered, already implements a very simple (and not very efficient) technique to manage the slots on the workbench in the warehouse. You can make and test this very simple implementation, and discover that you actually lose money on most of the three example orders provided to you if you use this strategy. In fact, this is about the worst you can do.

Your job is to modify the code in packer.c to implement a more efficient strategy that will make more money. The trick is to send back the bin that is least likely to be needed again in the near future. If you can do so, then you can reduce the cost of kits, decrease your cost and increase your profit.

To make things more interesting, you will also be given more credit if you use less memory in your implementation. It is possible to do a very good job using very small arrays of history data in your implementation.

You may add new functions and / or variables to packer.c, as well as modify the `processPart` function. When your code is graded, we will use unmodified versions of warehouse.c, warehouse.h, and packer.h; so any changes you make to any of those files will be lost.

The functions in warehouse.c are intentionally designed to prevent you from looking ahead on the order list. You must fill each order entry as it arrives, before looking at the next order entry.

I have given three sample orders in the files order1.txt, order2.txt and order3.txt. Your program should run with any valid order (arbitrary list of part numbers between 0 and 999), but any orders I test with will have the property that **part numbers tend to be sequential, but when the sequence is not followed, there is a high probability that the next part number showed up recently on the order.**

### Strategies to Think About

Even though you can't look ahead to see what part numbers are coming up in the future for a specific order, you do have information about the parts that have come up in the past. However, you don't know how many parts are in any given order (I have not limited the number of parts in an order), so you won't be able to table *all* the previous parts in the order.

If you think about it for awhile, you don't need the entire previous order. You may be able to get away with keeping statistics just on each part number (which has a limit of 1000.)Or you may want to keep statistics on each bin (you know there are exactly 100 bins altogether, so you can table info about each bin). Maybe you can even figure out an answer that only keeps track of the bins that are *currently* on the workbench.

Most important for success in this project is to use the single piece of information I have given you about part numbers yet to come in an order, namely: part numbers tend to be sequential, but when the sequence is not followed, there is a high probability that the next part number showed up recently on the order. Can you use this information to determine which slot on your workbench contains a bin that has the lowest probability of being needed again in the future? If so, when your workbench is full and you need a new bin, use that slot.

### Hints and Recommendations

1. Your workbench is initialized for each order by putting bin `-1` in each slot. If you find bin -1 in a slot, you know that that slot has not been used yet.

2. Any variable declared inside the `processOrder` function is a local variable by default, and gets re-created and re-initialized every time `processOrder` is called. 

   If you want to keep persistent information for the entire life of the program, you need to either make a global variable in `packer.c`, or declare a `static` variable inside the `processOrder` function. Use the `partIndex` function inside of `processOrder` to res-initialize such a variable for each order. For example,

   ```C
   if (partIndex()==0) {
     // Initialize static and global variables for the order here
   }
   ```
3. If you want to see more detail about how your workbench is being used, do a `make clean`, followed by `make warehouse_debug`. This will build a debug version of the warehouse executable that prints the bins on your bench every time you change those bins. Once you are finished debugging, do a `make clean` followed by a `make test` (or just `make`) to rebuild the non-debug version of the warehouse executable.

4. If your code isn't working as you intended, use the gdb debugger. See the [How To Use gdb](https://www.cs.binghamton.edu/~tbartens/HowTo/Using_gdb) page for more details.

5. One of the challenges of this assignment is figuring out when you are done. You should keep working on you code if you think you can come up with a strategy that does a better job, but there is no definitive way to determine if you have come up with the "best possible" algorithm. In the real programming world, there are often ambiguous assignments like this. The expected results will not be published.

## Submitting your Results

Commit your updates to packer.c in GitHub, and post the resulting GitHub hash code in the submission space for Project 1 in Brightspace. You may submit multiple times, only the latest submission before the assignment deadline will be graded.

## Grading Criteria

This assignment is worth a total of 100 points. After the due date, your code will be loaded onto a Computer Science (LDAP) server, and merged with the original infrastructure files (warehouse.c, warehouse.h, and packer.h.) Your code will then be built and run against the three published test cases (order1.txt, order2.txt, and order3.txt) as well as 2 unpublished test cases (order4.txt and order5.txt). Deductions will occur as follows:

- Your code will be compared against all other CS-220 student submissions. If you wrote your own project code, it will be different enough from other students so that there will be no problem - even if you worked as a team on the concepts. If you cut and paste your code, or if your code is copied by someone else, even if you modify variable names, comments, white space, etc. it will be considered plagiarism, and result in a zero grade.

- There will be a 10-point deduction for each 24 hours your assignment is late up to a maximum of 5 days. (After 5 days, you will receive a zero.)

- If you do not try to address the problem in this project (in other words, if you turn in unmodified code), you will get a 100 point deduction.

- If your code has a compiler errors or logic errors, I will try to fix your code. For each simple fix, I will make that fix, subtract up to 20 points (depending on how bad the bug is), and continue testing. If there is no simple fix, or more than 3 compiler or logic errors, you will get a grade of 40 for this project.

- If your code does not fill any of the published or unpublished orders, no matter what the resulting net earnings is, you will get a 15-point deduction per unfilled order.

- Your net *cost* will be compared to the minimum possible net cost for each order (without look-ahead). If your cost is greater than the minimum, you will get a deduction of up to 10 points per order, pro-rated based on the minimum cost. (If you are close to the minimum cost, only a point or two will be deducted, but if you are far off, nine or ten points may be deducted.)

- Up to 20 points may be deducted for using too much memory in your implementation. The deduction will be pro-rated based on how big the arrays are in your implementation. Very small arrays will result in no deductions for memory.
