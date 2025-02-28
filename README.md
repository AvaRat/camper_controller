# Camper Controller


## Overview
The project aims to create a usable system that can be mounted in a campervan, trailer, 4x4 overland vehicle or even small boat. It might be usefull in setups where 12V installation is present.
High level system design is presented in [CamperVan Control Unit system design.pdf](system design document) .


## How to interpret this repo
Project contains electronic parts as well as control logic and user interface source codes. I decided to keep everything in one repo as all parts are pretty much dependent on each other, and with this being one-person project keeping all parts in one place make perfect sense to me.

Please see [design decisions.md](design_decisions.md) for details regarding some of the decision made.






## Roadmap
One of the gratest inspirations for me regarding how to organise things is the Opulo project: [opulo](https://github.com/opulo-inc/lumenpnp/tree/main) I'm still far away from it, but that's something I'm aiming for with respect to project structure and organisation.

Ideally I would like to use github pipelines to create BOM from PCB design files. Also somehow linking pin assignment from Kicad into C code would be great. 

Besides, the project is still in the early phase and requires some time to bring it to working prototype phase.


