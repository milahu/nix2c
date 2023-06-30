# nix2c

transpile nix expressions to C code.

## status

failed experiment.

making evaluation faster is no solution for incremental evaluation,
because we still need full re-evaluation,
which is still slower than true incremental evaluation.

also, evaluation is still slow because Nix is a dynamic language,
so we need type checks on runtime.
we could add nonstandard type annotations via comments,
similar to [jsdoc types in javascript](https://www.typescriptlang.org/docs/handbook/jsdoc-supported-types.html),
to remove some type checks on runtime ...
but this would be still slower than true incremental evaluation.

we could make JIT faster by compiling straight to machine code,
just like the nodejs V8 engine ...
but this would be still slower than true incremental evaluation.

### incremental computing

im going back to [incremental computing](https://en.wikipedia.org/wiki/Incremental_computing).

also the [nickel roadmap](https://nickel-lang.org/user-manual/introduction#current-state-and-roadmap)
mentions "Incremental evaluation" as a goal:

<blockquote>

Incremental evaluation:
design an incremental evaluation model and a caching mechanism
in order to perform fast re-evaluation upon small changes to a configuration.

</blockquote>
