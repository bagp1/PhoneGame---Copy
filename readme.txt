Description
this is a UE4 game with a C++ character extension
Character's beginPlay spawns a thread that non-blockingly polls for UDP input
Blueprint polls the thread for input
Blueprint adds the difference between current and last inputs to the transform of the ghost sword
Blueprint uses physics to make the real, game interactable sword aproach the ghost sword

Warning
I can't seem to get the icon to not exclude and it's required to be in Build/Windows/ for the project to compile. I stuck it in the top level so move it before building.

TODO
add time since polled variable to scale input in blueprint based off when it was last accessed
Crashes during run due to a memory access issue
