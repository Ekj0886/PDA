#  Die-to-Die Global Routing

## Introduction
<div style="font-size: 15px">
Global Routing (GR) is an important step in the IC design flow used to determine the
<div style="font-size: 15px">
approximate paths for signal nets across the design space. This process mainly focuses on
<div style="font-size: 15px">
assigning "routing resources" and providing a "coarse routing solution" for subsequent 
<div style="font-size: 15px">
Detailed Routing (DR).
Its primary goals include:
<br>
<br>

* Path Planning:
Determine a global connection path for each signal net without specifying the exact 
placement of wires.

* Resource Allocation:
Ensure that routing resources (such as metal layers and routing tracks) are utilized efficiently across the design to avoid congestion.

* Feasibility Check:
Provide a congestion map to evaluate whether the design is routable and offer optimization suggestions.

* Performance Optimization: Minimize routing length and total delay while meeting requirements for timing, power, and reliability.

## Implement
* A* Search Algorithm
* Gcell data structure
* Efficient Openlist & Closelist data structure

## Result
<table>
  <tr>
    <td>
      <img src="testcase/testcase0/placement.png" alt="Image 1" width="1000">
    </td>
    <td>
      <img src="Image/testcase0.png" alt="Image 2" width="1000">
    </td>
  </tr>
</table>

<table>
  <tr>
    <td>
      <img src="testcase/testcase1/placement.png" alt="Image 1" width="1000">
    </td>
    <td>
      <img src="Image/testcase1.png" alt="Image 2" width="1000">
    </td>
  </tr>
</table>

<table>
  <tr>
    <td>
      <img src="testcase/testcase2/placement.png" alt="Image 1" width="1000">
    </td>
    <td>
      <img src="Image/testcase2.png" alt="Image 2" width="1000">
    </td>
  </tr>
</table>