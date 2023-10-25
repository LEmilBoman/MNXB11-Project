Work plan for group F.

Setup: Follow instructions in "Homework Tutorial 8 to Tutorial 11"
    1. CLI: Henrik
    2. CSV: Pavel
    3. Date: Emil
    4. ROOT: Nils
    5. fmt: optional (see if we need it)

Dataset:
    
Project parts:
    Data Preparation/Cleanup: Emil makes bash script.
    Emil: Compare number of days with winter temperatures (av. temp.<0 C) per year in Lund, Uppsala, and Luleå.
     Time period 1964-2022.
      Data Extraction: Bash script for extracting days with negative temperature.
                       Treat calculation of average temperatures and extraction of winter temperature days in c++.
      Data Analysis: Calculate moving average for the number of winterdays per year for each city.
                     Calculate relative change over time: (numb. WD in Lund/numb. WD in Luleå), (numb. WD in Uppsala/numb. WD in Luleå)
      Figure Generation: Plot with number of winterdays per year from 1964-2022 for all 3 cities, together with the respective moving average.
                         Plot with relative change over time for both Lund and Uppsala.
    Pavel: Look into Temperature delta between southern most avalable and northern most avalable city (Lund and Lulea).
      Data extraction: Use cleanup Bash script on data for both cities. Write a different script to find a common time interval for both cities.
      Data Analysis/Figure Generation: Construct plot for the average temperatuere over the year for the chosen time period for each of the cities. Find the delta of the temperature over the year and make a plot of the delta. Possibly do the same over smaller chunks of time if there exist a noticable change over time.
    2:
      Data Extraction
      Data Analysis
      Figure Generation
    3:
      Data Extraction
      Data Analysis
      Figure Generation
    4:
      Data Extraction
      Data Analysis
      Figure Generation




    
