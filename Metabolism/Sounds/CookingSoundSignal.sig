AudioSignalResClass {
 Inputs {
  IOPItemInputClass {
   id 14
   name "STOVE_HEAT"
   tl -647.5 -270.596
   children {
    23
   }
  }
  IOPItemInputClass {
   id 22
   name "Input 22"
   tl -646.154 -382.308
   children {
    23
   }
  }
  IOPInputValueClass {
   id 27
   name "Value 27"
   tl -383.077 -603.846
   children {
    26
   }
   value 2
  }
 }
 Ops {
  IOPItemOpSumClass {
   id 23
   name "Sum 23"
   tl -289.231 -378.462
   children {
    25 26
   }
   inputs {
    ConnectionClass connection {
     id 22
     port 0
    }
    ConnectionClass connection {
     id 14
     port 0
    }
   }
  }
  IOPItemOpDivClass {
   id 25
   name "Div 25"
   tl -99.333 -234.667
   children {
    2
   }
   inputs {
    ConnectionClass connection {
     id 23
     port 0
    }
   }
   Divisor 3
  }
  IOPItemOpMulClass {
   id 26
   name "Mul 26"
   tl -99.231 -518.407
   children {
    24
   }
   inputs {
    ConnectionClass connection {
     id 27
     port 0
    }
    ConnectionClass connection {
     id 23
     port 0
    }
   }
  }
 }
 Outputs {
  IOPItemOutputClass {
   id 2
   name "StoveVolume"
   tl 128.757 -210.564
   input 25
  }
  IOPItemOutputClass {
   id 24
   name "StovePitch"
   tl 98.462 -433.077
   input 26
  }
 }
 compiled IOPCompiledClass {
  visited {
   261 133 5 11 267 134 135 6
  }
  ins {
   IOPCompiledIn {
    data {
     1 3
    }
   }
   IOPCompiledIn {
    data {
     1 3
    }
   }
   IOPCompiledIn {
    data {
     1 131075
    }
   }
  }
  ops {
   IOPCompiledOp {
    data {
     2 65539 131075 4 65536 0 0 0
    }
   }
   IOPCompiledOp {
    data {
     1 2 2 1 0
    }
   }
   IOPCompiledOp {
    data {
     1 65538 4 131072 0 1 0
    }
   }
  }
  outs {
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
  }
  processed 8
  version 2
 }
}