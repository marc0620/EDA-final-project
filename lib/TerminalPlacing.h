class SquareofNet
{
    public:
     int llx,lly,hrx,hry;
     SquareofNet(){llx = lly = sentinel; hrx = hry = -1*sentinel;}
     int getmidx(){return (llx+hrx)/2;}
     int getmidy(){return (lly+hry)/2;}
};