# The Project Aims to create an ultimate control unit for my campervan.

While building my own van I came across multiple challenges in both domains technical and logistics. Off couse the main issue was the cost of the components that I wanted to purchase ;) 
That's why I started to build my own parts for it. I graduated robotics with a strong software focus, so the obvious direction for me was to  develop my own control board. 
There are a couple of off the shelve options, but none of then suits me well. Also designing a product from strach that I see a specific use case for is FUN.

First let's discuss market available options:
1. https://cbe.it/en/Standard-products/New-technology/Control-panels
2. https://simarine.net/via/
3. https://www.garmin.com/en-US/p/716468
4. https://www.garmin.com/en-US/p/861146/pn/010-02764-00
5. https://www.garmin.com/en-US/c/marine/digital-switching/

The rough price for such a system (central unit and display) can be between 200 and 1000 euro, so I have some room to play with.

The main functions I want to implement:
1. Enable user to switch power for multiple loads
2. Provide safety for 12V power distribution system
3. Enable user to control the system from multiple places (front and rear of the van, smartphone)
4. Grid connection detection so the system can change behaviour when ther's free power from the grid.
5. Provide intelligent lightning control (change of brightness, ideally color for mood-based scenarios)
6. Monitor core parameters like:
   - temperature outside
   - temperature inside
   - Main Battery State of Charge
   - Water left in main tank
   - Grey water tank status (if full)
