from math import sqrt
num = 0.0
newNum = 0.0
sumAll = 0.0
operator = ""
 
opVar = False
sumIt = 0
def Nums(self):
        global num
        global newNum
        global opVar
        
        sender = self.Dialog.sender()
         
        newNum = int(sender.text())
        setNum = str(newNum)
 
        if opVar == False:
            self.listView.setText(self.listView.text() + setNum)
        else:
            self.listView.setText(setNum)
            opVar = False
def pointClicked(self):
        global opVar
             
        if "." not in self.listView.text():
            self.listView.setText(self.listView.text() + ".")
def Switch(self):
        global num
             
        try:
                num = int(self.listView.text())
                 
        except:
                num = float(self.listView.text())
        num = num*(-1)
        numStr = str(num)
             
        self.listView.setText(numStr)
def Operator(self):
        global num
        global opVar
        global operator
        global sumIt
     
        sumIt += 1
     
        if sumIt > 1:
                Equal(self)
     
        num = self.listView.text()
     
        sender = self.Dialog.sender()
     
        operator = sender.text()
             
        opVar = True
def Equal(self):
        global num
        global newNum
        global sumAll
        global operator
        global opVar
        global sumIt
     
        sumIt = 0
     
        newNum = self.listView.text()
     
        print(num)
        print(operator)
        print(newNum)
             
        if operator == "+":
            sumAll = float(num) + float(newNum)
     
        elif operator == "-":
            sumAll = float(num) - float(newNum)
     
        elif operator == "/":
            sumAll = float(num) / float(newNum)
     
        elif operator == "*":
            sumAll = float(num) * float(newNum)
        elif operator == "pow":
            sumAll = float(num) ** float(newNum)
                 
        print(sumAll)
        self.listView.setText(str(sumAll))
        opVar = True
def AC(self):
        global newNum
        global sumAll
        global operator
        global num
             
        self.listView.clear()
     
        num = 0.0
        newNum = 0.0
        sumAll = 0.0
        operator = ""
def Sqrt(self):
        global num
             
        num = float(self.listView.text())
        if (num<0):
            self.listView.setText('ERROR')
        n = sqrt(num)
        num = n
     
        self.listView.setText(str(num))
def Pow(self):
        global num
        global opVar
        global operator
        global sumIt
     
        sumIt += 1
     
        if sumIt > 1:
            self.Equal()
     
        num = self.listView.text()
     
        sender = self.Dialog.sender()
     
        operator = sender.text()
             
        opVar = True

