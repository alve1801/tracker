# tracker
Preemptive scheduling based chiptune tracker

Sound synthesizer based on [the other one](https://github.com/alve1801/synth), yet instead of complex signal processing, this one features complex track concurrency. Explained in more detail on [my website](https://avethenoul.neocities.org/tracker.html). Compile with `gcc main.c -lm -funsigned-char`, call with `./a.out <track_file> | aplay` or include a dummy argument for debug output (like with the other synth).

Actual tracks are in a binary format. The project also features a compiler from a custom, assembly-like format to said binary (also included here). Compile with `gcc -o comp comp.c -funsigned-char`, use standard io for input/output (ie call with `./comp <in_file >out_file`)

Like the other synth, this is published under CC-BY-SA-NC, or ask for permission.
