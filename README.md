# Algorithmic trading

Implementation of the Algorithmic trading project.


## Contents

1. [Chapter I](#chapter-i) \
    1.1. [Introduction](#introduction)
2. [Chapter II](#chapter-ii) \
    2.1. [Interpolation](#interpolation)  
    2.2. [Newton interpolation polynomial](#newton-interpolation-polynomial)  
    2.3. [Cubic spline interpolation](#cubic-spline-interpolation)  
    2.4. [Approximation](#approximation)  
    2.5. [Least Squares Method](#least-squares-method)
3. [Chapter III](#chapter-iii) \
    3.1. [Interpolation of tabulated functions with cubic spline interpolation](#part-1-interpolation-of-tabulated-functions-with-cubic-spline-interpolation)  
    3.2. [Approximation of tabulated functions](#part-2-approximation-of-tabulated-functions)  

## Chapter I 

## Introduction

In this project you will learn about interpolation and approximation algorithms. Using these algorithms, you will be able to plot the probabilistic behavior of stock quotes and their price forecasts for the foreseeable future based on historical data without regard to external factors __*__.


## Chapter II

### Interpolation

*Interpolation* is a method of finding intermediate values of a quantity based on a discrete set of known values.

Function interpolation is used when you want to find the value of the function $`y(х)`$ at the value of the argument $`x`$ that belongs to the interval $`[x_0, ..., x_n]`$ ($`x_0, ..., x_n`$ are values given in the table), but does not coincide with any of the known values of this interval.

In many cases, the analytical expression of the function $`y(x)`$ is not known and it is not possible to find the value of this function from the table of its values.
So, instead of it another function $`f(x)`$ is plotted, which is easy to calculate and has the same table of values (coincides with it at the points $`x_0, x_1, x_2,..., x_n`$) as $`y(x)`$.

Finding an approximate function is called interpolation and points $`x_0, x_1, x_2, ..., x_n`$ are interpolation nodes. Usually the interpolating function is found as a polynomial of $`n`$ degree.

### Newton interpolation polynomial

If the interpolation nodes are equally spaced, so that $`x_i + 1 - x = h = const`$, where $`h`$ is the interpolation step, i.e. $`x_i = x_0 + n * h`$, then the interpolation polynomial can be written in the form proposed by Newton.

Newton's first interpolation formula is found in the form: $`Pn(x) = a_0 + a_1(x - x_0) + a_2(x - x_0)(x - x_1) + ... + a_n(x - x_0)...(x - x_n)`$.

### Cubic spline interpolation

Cubic spline interpolation is a special case of piecewise polynomial interpolation. In this special case, the function is interpolated by a cubic polynomial between any two neighboring nodes.
Its coefficients at each interval are determined from the conjugation conditions in the nodes:

$`f_i = y_i, f'(x_i - 0) = f'(x_i + 0), f''(x_i - 0) = f''(x_i + 0), i = 1, 2, ..., n - 1`$, where $`x_i + 0`$ and $`x_i - 0`$ - the neighborhood of point $`x_i`$.

In addition, there are conditions at the boundary at $`x = x_0`$ and $`x = x_n`$:
$`f''(x_0) = 0, f''(x_n) = 0`$

### Approximation

*Approximation* is a replacement of some mathematical objects by others, in one sense or another, close to the original ones.

When interpolating, the interpolating function strictly passes through the table nodes because the number of coefficients in the interpolating function is equal to the number of table values.
Approximation is a method in which the approximated function passes not through the interpolation nodes, but between them in order to find additional values that are different from the table data.

If the analytical expression of the function is unknown or very complex, then we have to find an empirical formula, the values of which would not differ much from the experimental data.

### Least Squares Method

The purpose of the least squares method is to find such values of $`x_i`$ at which the sum of the squares of deviations (errors) $`e_i = y_i - f_i(x)`$ will tend to a minimum.

Since each value of $`x_i`$ is generally "accompanied" by the corresponding coefficient $`a_i (i = 0, 1, 2, ..., n)`$, the problem is reduced to finding these coefficients.


## Chapter III

## Part 1. Interpolation of tabulated functions with cubic spline interpolation

You need to plot a tabulated function of stock quotes using interpolation methods:

- The program must be developed in C++ language of C++17 standard
- The library code must be located in the src folder in the develop branch
- When writing code it is necessary to follow the Google style
- The program must be built with Makefile which contains standard set of targets for GNU-programs all, install, uninstall, clean, dvi, dist. Installation directory could be arbitrary, except the building one
- GUI implementation, based on any GUI library with API for C++ (Qt, SFML, GTK+, Nanogui, Nngui, etc.)
- The data are loaded into the program from a file with stock quotes with the .csv extension:
    -  The file contains data in the form of a table, where the first column is the date, the second column is the value of the function (examples of data files are in the materials folder)
- When uploading a new data file, clear the field for drawing graphs
- The user sets the number of points on which the graph should be plotted (the number of points is not less than in the loaded file)
- All points are evenly distributed between the start and end dates
- On the final graph adjacent points must be connected by a straight line
- The *Cubic Spline* interpolation method must be implemented.
- There should be a button in the interface for drawing the cubic spline graph
- There should be a button in the interface for clearing the field to draw graphs (the field is cleared only when you click on this button or new data is uploaded)
- There can be up to 5 graphs displayed in the field at the same time (all graphs have different color)
- If there are already 5 graphs displayed in the field, the buttons for drawing new graphs must be blocked
- The interface has to contain the following information about the graphs:
    - Color
    - Name of the file from which the data were taken
- Provide the ability to output the values of the stock quotes function obtained by interpolation method according to a user-defined argument value (date and time)

## Part 2. Approximation of tabulated functions

You need to plot a tabulated function of stock quotes using the *least squares method*. \
Moreover, the graph should cover a longer period of time than the input data:

- For graphs from this task there should be a separate field in the interface for drawing
- When uploading a new data file, clear the field for drawing graphs
- The user sets the number of points on which the graph should be plotted (the number of points is not less than in the loaded file)
- All points are evenly distributed between the start and end dates
- On the final graph adjacent points must be connected by a straight line
- On the graph, the points specified in the loaded file must be visually marked:
    - The radius of these points is larger than the thickness of the graph curve
    - The color of these points are differ from colors of the graphs in the field
- The user sets the number of days `M` for which we want to extend the graph (how many days ahead we want to predict the stock price)
- Clear the drawing field when adding a new approximation graph, in which `M` differs from `M` of already drawn graphs
- There can be up to 5 graphs with the same value of the number of days `M` displayed in the field at the same time
  (all graphs should have different color)
- There should be a button in the interface for drawing the graph plotted by the polynomial of the degree set at that time
- There should be a field in the interface for entering the degree of the polynomial
- There should be a button in the interface for clearing the field to draw the approximation graphs
- The interface has to contain the following information about the graphs:
    - Color
    - Name of the file from which the data were taken
    - Degree of polynomial
- If there are already 5 graphs displayed in the field, the buttons for drawing new graphs must be blocked
- Provide the ability to display the value of the approximating function for a given degree of the polynomial according to the user-defined value of the argument (date and time)
