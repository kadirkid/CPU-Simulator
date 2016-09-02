# CPU-Simulator
<p>
<b><mark>Phase 1:</mark></b><br>
This code loads, adds and stores any set of numbers. There are 4096 (0-4095) spots in the unsigned RAM array 
which stores the data and instructions. The data is stored from 2048 to 3017, which is where the final sum is 
stored, while the instructions are stored from 1024 to 2047. The rest is given to the OS, Heap and Stack. 
The OS is stored from 0 to 1023 while the Heap and Stack are stored from 3072 to 4095. The data and instructions
are saved on a text file, in which they are then stored in their allocated places on the RAM array. Every time 
an instruction is loaded, a counter is incremented.<br>

When the inputting phase is complete, the pc is set to the starting address (location of first instruction). 
After that the program executes a loop where the instructions is fetched, decoded, used to calculate the address
and executed until the condition to end the loop is met. <br>

When the loop is done executing, the result, which is stored in location 3071 in the RAM, is then used to 
calculate the average by dividing it with the number from location 2048 in the RAM. The Sum and Average is
then printed.<br>

</p>
<p>
<b><mark>Phase 2:</mark></b><br>
This phase is similar to phase 1 but we take a different approach in getting the final output. Instead of 
taking the instructions from the RAM, we take Microinstructions from the ROM. Microinstructions deal with 
the 16 registers individually, setting up the values required to get the desired result. Instead of the fetch,
decode, calculate and execute functions, we divide it into 4 sub cycles. <br>

In the 1st sub cycle, the MIR is taken from the ROM and is then broken down to get the values of the AMUX, 
COND,  ALU, SH, MBR, MAR, RD, WR, ENC, C, B, A and ADDR by masking it.<br>

In the 2nd sub cycle, we open the A latch and B latch to allow the values from the A bus and B bus through.<br>

In the 3rd sub cycle,  it sets the value for the MAR and reads the value at the address specified and loads 
it into the MBR.<br>

In the 4th sub cycle,  all the calculations are carried out (AMUX -> ALU -> Shifter -> C bus) and a value is
sent to the 16 registers, stating which register the value from the C bus passes through. It also send values
to the MPC, MBR and C decoder.<br>

My code, however, did not work.<br>


