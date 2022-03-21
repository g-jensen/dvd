# DVD Bounce with C++/SFML + ImGui

The collision is not simulated, its done by a mathematical equation with the length and width of the rectangle as parameters.  

In the source code for this program, you can find a function to simulate collision which is what I used to figure out the equation but the function is never used.  

---

Parametric equations for x and y given l = length and w = width:  

x(t) = abs(2 * abs(mod(t + l - (l/2),2 * l) - l) - l)  
y(t) = abs(2 * abs(mod(t + w - (w/2),2 * w) - w) - w)  

This assumes starting at the bottom left of the rectangle at a 45 degree angle.  

---

Running the program:

```
./main
```
is fine but you can also add 2 parameters being the length and width of the rectangle  

Example:  
```
./main 500 300
```

---

Controls:  

Arrow keys to move the camera around.  
PageUp and PageDown to zoom in and out respectively.  
Spacebar to pause

---

Credits:  

SFML: opening window, drawing stuff  
Dear ImGui: GUI boxes  
imgui-sfml: compatibility between SFML and ImGui