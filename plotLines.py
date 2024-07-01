import matplotlib.pyplot as plt 
  
f = open(r"D:\\edgeDumpnew.txt","r+")
f2 = open(r"D:\\edgeDumpwithScratch.txt","r+")

x=[]
y=[]
a= 0

for i in f.readlines():
    a = a+1
# create data 
    x.clear()
    y.clear()
    temp = i.split(",")
    x.append(float(temp[0].strip("\n")))
    x.append(float(temp[2].strip("\n")))
    y.append(float(temp[1].strip("\n")))
    y.append(float(temp[3].strip("\n")))
    if(temp[4].strip("\n")== "remove"):
        plt.plot(x,y, label = "line"+ str(a),linestyle="-.") 
    else: 
        plt.plot(x,y, label = "line"+ str(a)) 


# plot line 

plt.show()
