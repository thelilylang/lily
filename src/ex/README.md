# Externs

> This directory contains the externs for the external functions that are used in the project, like inline functions. This avoids the need for multiple linkage definitions, and avoids wondering whether you've already declared the inline function you're using as an extern. So every time you define a new inline function that isn't declared with static, you need to add it to the file corresponding to the library to which the file you're modifying belongs.
