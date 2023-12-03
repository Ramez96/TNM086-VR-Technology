from H3D import *
from H3DInterface import *

#mat = references.getValue()
import random
class colorSwitch(TypedField(SFColor, MFBool)):

    def update(self, event):

        if len(event.getValue()) > 0 :
            return RGB(random.randint(0,255)/255, random.randint(0,255)/255, random.randint(0,255)/255)
        else:
            return RGB(1.0, 1.0, 1.0)
        
color = colorSwitch()