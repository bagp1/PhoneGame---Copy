this is a UE4 game with a C++ character extension
Character's beginPlay spawns a thread that non-blockingly polls for UDP input
Blueprint polls the thread for input
Blueprint adds the difference between current and last inputs to the transform of the ghost sword
Blueprint uses physics to make the real, game interactable sword aproach the ghost sword

TODO
add time since polled variable to scale input in blueprint based off when it was last accessed
Crashes during run due to a memory access issue
