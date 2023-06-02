# Cell-Simulation
I took this project from conception to completion in hopes that someone could find this tool useful. The goal of this project was to create a way to simulate cells and viruses, allowing us to predict their behavior. Depending on the parameters, you get some pretty interesting behaviors.

This project was written in C++. The first UI box was generated from wxWidgets and the simulation was done with the SFML library.

## Simulation Parameters:
White Blood Cells, Red Blood Cells, Viruses - Aggression, Infection, Production, Bone Marrow Supply and more.
The Viruses has a Lysogenic feature, in which the viruses actually attack WBC/RBC and is able to reproduce.
For the more regular illnesses, a Lysogenic value of 0 indicates that it will not attack the cell until it dies and will just allow the cell to essentially do nothing towards the virus. However, the cells can still be made aggressive towards other cells, including towards others in it's own group (WBC/RBC).
