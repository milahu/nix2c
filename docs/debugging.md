https://stackoverflow.com/questions/63702046/how-to-debug-and-step-into-custom-language-sources-transpiled-to-c

Your transpiler should put this directive for each source line in the original file into the generated file:

```c
#line 3 "source.srclang"
// generated code
```
