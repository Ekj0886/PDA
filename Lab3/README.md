# Legalization of Multi-bit FF Banking Placement

## Introduction
>Many optimization techniques can result in cell overlap, leading to illegal placement. Therefore, a fast algorithm is needed to perform local legalization rather than full-chip legalization and determine whether the optimization is feasible at that moment. Consequently, a quick API for the legalizer is necessary so that the optimizer can frequently check whether the optimization is easy to legalize.

## Proposed Algorithm => [Report](313510171_rpt.pdf)
<ul style="font-size: 20px;">
  <li>FindVacant</li>
  <img src="image/FindVacant.png" alt="Image 1" width="600">
  <br>
  <br>
  <li>FindSwap</li>
  <img src="image/FindSwap.png" alt="Image 1" width="600">
  <br>
  <br>
  <li>Other Features</li>
  <img src="image/Other.png" alt="Image 1" width="600">
</ul>

## Result
* <div style="font-size: 18px"> testcase1_16900 </div>
<table>
  <tr>
    <td>
      <img src="Layout_Image/testcase1_16900.png" alt="Image 1" width="3000">
      <p style="text-align: center; font-size: 18px">Original Placement</p>
    </td>
    <td>
      <img src="Layout_Image/testcase1_16900_post.png" alt="Image 2" width="3000">
      <p style="text-align: center; font-size: 18px">After Banking</p>
    </td>
  </tr>
</table>

* <div style="font-size: 18px"> testcase1_ALL0_5000 </div>
<table>
  <tr>
    <td>
      <img src="Layout_Image/testcase1_ALL0_5000.png" alt="Image 1" width="3000">
      <p style="text-align: center; font-size: 18px">Original Placement</p>
    </td>
    <td>
      <img src="Layout_Image/testcase1_ALL0_5000_post.png" alt="Image 2" width="3000">
      <p style="text-align: center; font-size: 18px">After Banking</p>
    </td>
  </tr>
</table>

* <div style="font-size: 18px"> testcase2_100 </div>
<table>
  <tr>
    <td>
      <img src="Layout_Image/testcase2_100.png" alt="Image 1" width="3000">
      <p style="text-align: center; font-size: 18px">Original Placement</p>
    </td>
    <td>
      <img src="Layout_Image/testcase2_100_post.png" alt="Image 2" width="3000">
      <p style="text-align: center; font-size: 18px">After Banking</p>
    </td>
  </tr>
</table>

* <div style="font-size: 18px"> Hellish testcase </div>
<table>
  <tr>
    <td>
      <img src="Layout_Image/testcase1_MBFF_LIB_7000.png" alt="Image 1" width="3000">
      <p style="text-align: center; font-size: 18px">Original Placement</p>
    </td>
    <td>
      <img src="Layout_Image/testcase1_MBFF_LIB_7000_post.png" alt="Image 2" width="3000">
      <p style="text-align: center; font-size: 18px">After Banking</p>
    </td>
  </tr>
</table>