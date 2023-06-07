# Cell-Simulation by Thanh La
I took this project from conception to completion in hopes that someone could find this tool useful. The goal of this project was to create a way to simulate cells and viruses, allowing us to predict their behavior. Depending on the parameters, you get some pretty interesting behaviors.

This project was written in C++. The first UI box was generated from wxWidgets and the simulation was done with the SFML library.

## Simulation Parameters:
- White Blood Cells, Red Blood Cells, Viruses - Aggression, Infection, Production, Bone Marrow Supply and more.
- WBC - white center
- RBC - red center
- Virus - green center
- Ring Colors
- - Yellow : non-aggressive, Red : aggressive, Grey : Mutated

## Notable Features:
- There are preset parameters to simulate the common diseases 
- The Viruses have a Lysogenic feature, in which the viruses actually attack WBC/RBC and is able to reproduce. A lysogenic value of 0 indicates that it will not attack the cell until it dies and will just allow the cell to essentially do nothing towards the virus. However, the cells can still be made aggressive towards other cells, including towards others in it's own group (WBC/RBC).
- Aggression levels are set at a percentage, where a random number generator determines if it will attack or not. Note, viruses will always attack if aggression is set to 1 ( e {0, 1} ), so this feature will not be up to chance. The same applies for the WBC and Virus interaction, if the WBC is not infected, it will always attack. Where as the WBC and RBC interactions are.
- Aggression values, pre and post infection will help fine tune the behavior you will model.
- Flashing units signify that they are being under attack.
- The viruses has a threshold value where the population size must qualify the threshold value before it is recognized as a threat.
- When a cell that isn't infected dies off, the WBC will all go towards the spot where the RBC/WBC died to defend the system.
- Bone marrow supply, replenish rate and the cell spawn rate all work together to emulate a cell production system. When the cells fall below the stable threshold, the system will pump out the maximum that it can. The maximum is barred by the spawn rate and the bone marrow supply.
- For testing and those who want to see what the paths considered for finding an enemy within range looks like, you can use the highlight_map function in the Simulation file. This function takes in a population map and color for what the paths you want to show.
- Foreign cells will only be attacked when their population size qualifies the threshold.
- The whiter the ring is around the virus, the higher the mutation version.

### Parameter Panel
<img width="994" alt="Screenshot 2023-06-02 at 1 44 29 AM" src="https://github.com/Thanh-sies/Cell-Simulation/assets/7445307/66923ea5-6cd9-4e79-b257-596b5e8b76cc">

### Highlight path debug function
<img width="364" alt="Screenshot 2023-06-02 at 2 06 36 AM" src="https://github.com/Thanh-sies/Cell-Simulation/assets/7445307/2ca6454a-427f-4190-b175-fbab1fd6da4b">


### Video of the HIV Simulation
- The ring color around the cells can be swapped out based on what is easier for your liking. In the video below, mutated viruses have their ring turn black. In my updated code, I have the ring color advance toward white. This was an aesthetic change in case it would be harder to see. Just like the ring color, all other aspects of the animation can be changed based on the user's liking.

https://github.com/Thanh-sies/Cell-Simulation/assets/7445307/b29e5c41-ad7c-4ff4-b93f-1f750c1aac91

