On the Bresenham algorithm as implemented by Marlin:
(Taken from (https://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html)

The basic Bresenham algorithm:

Consider drawing a line on a raster grid where we restrict the allowable slopes of the line to the range 0 <= m <= 1

If we further restrict the line-drawing routine so that it always increments x as it plots, it becomes clear that, having plotted a point at (x,y), the routine has a severely limited range of options as to where it may put the next point on the line:

- It may plot the point (x+1,y), or:
- It may plot the point (x+1,y+1).

So, working in the first positive octant of the plane, line drawing becomes a matter of deciding between two possibilities at each step.

We can draw a diagram of the situation which the plotting program finds itself in having plotted (x,y).

```
  y+1 +--------------*
      |             /
      |            /
      |           /
      |          /
      |    y+e+m*--------+-
      |        /| ^    |
      |       / | |m   |
      |      /  | |    |
      |     /   | v    |
      | y+e*----|-----   |m+ε
      |   /|    | ^    |
      |  / |    | |ε   |
      | /  |    | |    |
      |/   |    | v    v
    y *----+----+----------+--
           x   x+1
```

In plotting (x,y) the line drawing routine will, in general, be making a compromise between what it would like to draw and what the resolution of the stepper motors actually allows it to draw. Usually the plotted point (x,y) will be in error, the actual, mathematical point on the line will not be addressable on the pixel grid. So we associate an error, ε, with each y ordinate, the real value of y should be y+ε . This error will range from -0.5 to just under +0.5.

In moving from x to x+1 we increase the value of the true (mathematical) y-ordinate by an amount equal to the slope of the line, m. We will choose to plot (x+1,y) if the difference between this new value and y is less than 0.5

```
y + ε + m < y + 0.5
```

Otherwise we will plot (x+1,y+1). It should be clear that by so doing we minimize the total error between the mathematical line segment and what actually gets drawn on the display.

The error resulting from this new point can now be written back into ε, this will allow us to repeat the whole process for the next point along the line, at x+2.

The new value of error can adopt one of two possible values, depending on what new point is plotted. If (x+1,y) is chosen, the new value of error is given by:

```
ε[new] = (y + ε + m) - y
```

Otherwise, it is:

```
ε[new] = (y + ε + m) - (y + 1)
```

This gives an algorithm for a DDA which avoids rounding operations, instead using the error variable ε to control plotting:

```
  ε = 0, y = y[1]
  for x = x1 to x2 do
    Plot point at (x,y)
    if (ε + m < 0.5)
      ε = ε + m
    else
      y = y + 1, ε = ε + m - 1
    endif
  endfor
```

This still employs floating point values. Consider, however, what happens if we multiply across both sides of the plotting test by Δx and then by 2:

```
        ε + m < 0.5
    ε + Δy/Δx < 0.5
2.ε.Δx + 2.Δy < Δx
```

All quantities in this inequality are now integral.

Substitute ε' for ε.Δx . The test becomes:

```
2.(ε' + Δy) < Δx
```

This gives an integer-only test for deciding which point to plot.

The update rules for the error on each step may also be cast into ε' form. Consider the floating-point versions of the update rules:

```
ε = ε + m
ε = ε + m - 1
```

 Multiplying through by Δx yields:

```
ε.Δx = ε.Δx + Δy
ε.Δx = ε.Δx + Δy - Δx
```

Which is in ε' form:

```
ε' = ε' + Δy
ε' = ε' + Δy - Δx
```

Using this new ``error'' value, ε'  with the new test and update equations gives Bresenham's integer-only line drawing algorithm:

```
ε' = 0, y = y[1]
for x = x1 to x2 do
  Plot point at (x,y)
  if (2.(ε' + Δy) < Δx)
    ε' = ε' + Δy
  else
    y = y + 1, ε' = ε' + Δy - Δx
  endif
endfor
```

It is a Integer only algorithm - hence efficient (fast). And the Multiplication by 2 can be implemented by left-shift.  0 <= m <= 1

### Oversampling Bresenham algorithm:

Even if Bresenham does NOT lose steps at all, and also does NOT accumulate error, there is a concept i would call "time resolution" - If the quotient between major axis and minor axis (major axis means, in this context, the axis that must create more step pulses compared with the other ones, including the extruder)

Well, if the quotient result is not an integer, then Bresenham, at some points in the movement of the major axis, must decide that it has to move the minor axis. It is done in such way that after the full major axis movement has executed, it also has executed the full movements of the minor axis. And the minor axis steps were properly distributed evenly along the major axis movement. So good so far.

But, as said, Bresenham has "discrete" decision points: It can only decide to move (or not to move) minor axis exactly at the moment the major axis moves. And that is not the ideal point (in time) usually.

With slow movements that are composed of a similar, but not equal number of steps in all axes, the problem worsens, as the decision points are distributed very sparsely, and there are large delays between those decision points.

It is nearly trivial to extend Bresenham to "oversample" in that situation: Let's do it:

Assume that we want to use Bresenham to calculate when to step (move in Y direction), but we want to do it, not for integer increments of the X axis, rather than, for fractional increments.

Let's call 'r' the count of subdivisions we want to split an integer increment of the X axis:

```
m = Δy/Δx = increment of y due to the increment of x1
```

Every time we move `1/r` in the X axis, then the Y axis should move `m.1/r`

But, as stated previously, due to the resolution of the screen, there are 2 choices:

- It may plot the point `(x+(1/r),y)`, or:
- It may plot the point `(x+(1/r),y+1)`.

That decision must be made keeping the error as small as possible:

```
-0.5 < ε < 0.5
```

So, the proper condition for that decision is (`m/r` is the increment of y due to the fractional `1/r` increment of `x`):

```
y + ε + m/r < y + 0.5
ε + m/r < 0.5                                   [1]
```

Once we did the decision, then the error update conditions are:

Decision A:
```
ε[new] = y + ε + m/r - y
ε[new] = ε + m/r                                [2]
```

Decision B:
```
ε[new] = y + ε + m/r - (y+1)
ε[new] = ε + m/r - 1                            [3]
```

We replace m in the decision inequality [1] by its definition:

```
ε + m/r < 0.5
ε + ΔY/(ΔX*r) < 0.5
```

Then, we multiply it by `2.Δx.r`:

```
ε + ΔY/(ΔX*r) < 0.5
2.ΔX.ε.r + 2.ΔY < ΔX.r
```

If we define `ε' = 2.ε.ΔX.r` then it becomes:

```
ε' + 2.ΔY < ΔX.r                                [4]
```

Now, for the update rules, we multiply by 2.r.ΔX

```
ε[new] = ε + m/r
2.r.ΔX.ε[new] = 2.r.ΔX.ε + 2.r.ΔX.ΔY/ΔX/r
2.r.ΔX.ε[new] = 2.r.ΔX.ε + 2.ΔY
ε'[new] = ε' + 2.ΔY                             [6]
```

```
ε[new] = ε + m/r - 1
2.r.ΔX.ε[new] = 2.r.ΔX.ε + 2.r.ΔX.ΔY/ΔX/r - 1 . 2.r.ΔX
2.r.ΔX.ε[new] = 2.r.ΔX.ε + 2.ΔY - 2.ΔX.r
ε'[new] = ε' + 2.ΔY - 2.ΔX.r                    [7]
```

All expressions, the decision inequality [4], and the update equations [5] and [6] are integer valued. There is no need for floating point arithmetic at all.

Summarizing:

```
Condition equation:

  ε' + 2.ΔY < ΔX.r                              [4]

Error update equations:

  ε'[new] = ε' + 2.ΔY                           [6]

  ε'[new] = ε' + 2.ΔY - 2.ΔX.r                  [7]
```

This can be implemented in C++ as:

```cpp
  class OversampledBresenham {
    private:
      long divisor, // stepsX
           dividend, // stepsY
           advanceDivisor, // advanceX
           advanceDividend; // advanceY
      int errorAccumulator; // Error accumulator

    public:
      unsigned int ticker;

      OversampledBresenhan(const long& inDividend, const long& inDivisor, int rate) {
        ticker = 0;
        divisor = inDivisor;
        dividend = inDividend;
        advanceDivisor = divisor * 2 * rate;
        advanceDividend = dividend * 2;
        errorAccumulator = -divisor * rate;
      }

      bool tick() {
        errorAccumulator += advanceDividend;
        const bool over = errorAccumulator >= 0;
        if (over) {
          ticker++;
          errorAccumulator -= advanceDivisor;
        }
        return over;
      }
  };
```
