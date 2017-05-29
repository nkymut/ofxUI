/**********************************************************************************
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#include "ofxUILabelToggleMatrix.h"
#include "ofxUI.h"



ofxUILabelToggleMatrix::ofxUILabelToggleMatrix(float x, float y, float w, float h, int _rows, int _cols, string _name, int _size) : ofxUIWidget()
{
    init(x, y, w, h, _rows, _cols, _name, _size);
}

ofxUILabelToggleMatrix::ofxUILabelToggleMatrix(float w, float h, int _rows, int _cols, string _name, int _size) : ofxUIWidget()
{
    init(0, 0, w, h, _rows, _cols, _name, _size);
}

void ofxUILabelToggleMatrix::init(float x, float y, float w, float h, int _rows, int _cols, string _name, int _size)
{
    initRect(x,y,w,h);
    name = string(_name);
    kind = OFX_UI_WIDGET_TOGGLEMATRIX;
    rows = _rows;
    cols = _cols;
    draw_back = false;
    
    toggleWidth = w;
    toggleHeight = h;
    
    for(int j = 0; j < rows; j++)
    {
        for(int i = 0; i < cols; i++)
        {
            //string  tglLabel = (name+"("+ofxUIToString(i,0)+","+ofxUIToString(j,0)+")");
             string tglLabel = name+ofxUIToString(j*cols+i);

            ofxUILabelToggle *toggle = new ofxUILabelToggle(tglLabel, false, toggleWidth, toggleHeight, 0, 0, OFX_UI_FONT_SMALL);

            addEmbeddedWidget(toggle);
            toggles.push_back(toggle);
        }
    }
    allowMultiple = true;
}

void ofxUILabelToggleMatrix::setVisible(bool _visible)
{
    visible = _visible;
    for(unsigned int i = 0; i < toggles.size(); i++)
    {
        ofxUILabelToggle *t = toggles[i];
        t->setVisible(visible);
        t->getLabelWidget()->setVisible(false);
    }
}

void ofxUILabelToggleMatrix::activateToggle(string _name)
{
    for(unsigned int i = 0; i < toggles.size(); i++)
    {
        ofxUILabelToggle *t = toggles[i];
        if(!(t->getName().compare(_name.c_str())))
        {
            t->setValue(true);
        }
        else
        {
            t->setValue(false);
        }
    }
}

void ofxUILabelToggleMatrix::setFont(ofxUIFont *_font){
   uiFont = _font;
   for(unsigned int i = 0; i < toggles.size(); i++)
   {
      ofxUILabelToggle *t = toggles[i];
      t->setFont(uiFont);
   }
   
}

void ofxUILabelToggleMatrix::setLabelVisible(bool _visible){
   for(unsigned int i = 0; i < toggles.size(); i++)
   {
      ofxUILabelToggle *t = toggles[i];
      t->setLabelVisible(_visible);
   }
   
}

void ofxUILabelToggleMatrix::setLabelText(string _name, string _value){

   for(unsigned int i = 0; i < toggles.size(); i++)
   {
      ofxUILabelToggle *t = toggles[i];
      if(!(t->getName().compare(_name.c_str())))
      {
         t->setLabelText(_value);
      }
   }

}

void ofxUILabelToggleMatrix::setLabelText(string _value, int x, int y){
   
   ofxUILabelToggle *t = toggles[y*cols + x];
   t->setLabelText(_value);
   
}

void ofxUILabelToggleMatrix::setParent(ofxUIWidget *_parent)
{
    parent = _parent;
    
    float tWidth = cols*(toggles[0]->getRect()->getWidth())+(cols-1)*padding;
    float tHeight = rows*(toggles[0]->getRect()->getHeight())+(rows-1)*padding;
    
    ofPoint pos = ofPoint(0,0);
    for(int j = 0; j < rows; j++)
    {
        for(int i = 0; i < cols; i++)
        {
            int index = i+j*cols;
            ofxUILabelToggle *t = toggles[index];
            t->getRect()->setX(pos.x);
            t->getRect()->setY(pos.y);
            t->setLabelVisible(false);
            t->setParent(this);            
            pos.x += toggleWidth+padding;
        }
        pos.y += toggleHeight+padding;
        pos.x = 0;
    }
    
    rect->setWidth(tWidth);
    rect->setHeight(tHeight);
    calculatePaddingRect();
}

void ofxUILabelToggleMatrix::setAllToggles(bool _value, bool _trigger)
{
    for(unsigned int i = 0; i < toggles.size(); i++)
    {
        ofxUILabelToggle *t = toggles[i];
        t->setValue(_value);
        if(_trigger)
        {
            parent->triggerEvent(t);
        }
    }
    parent->triggerEvent(this);
}

void ofxUILabelToggleMatrix::setToggle(unsigned int row, unsigned int col, bool _value, bool _trigger)
{
    ofxUILabelToggle *t = getToggle(row, col);
    if(t != NULL)
    {
        t->setValue(_value);
        if(_trigger)
            parent->triggerEvent(t);
    }
    parent->triggerEvent(this);
}

ofxUILabelToggle * ofxUILabelToggleMatrix::getToggle(unsigned int row, unsigned int col)
{
    if(row*col < toggles.size())
    {
        return toggles[col+row*cols];
    }
    return NULL;
}

bool ofxUILabelToggleMatrix::getState(int row, int col)
{
    ofxUILabelToggle *t = getToggle(row, col);
    if(t != NULL)
    {
        return t->getValue();
    }
    return false;
}

int ofxUILabelToggleMatrix::getColumnCount()
{
    return cols;
}

int ofxUILabelToggleMatrix::getRowCount()
{
    return rows;
}

vector<ofxUILabelToggle *> ofxUILabelToggleMatrix::getToggles()
{
    return toggles;
}

vector<ofxUILabelToggle *> * ofxUILabelToggleMatrix::getTogglesPtr()
{
    return &toggles;
}

void ofxUILabelToggleMatrix::triggerEvent(ofxUIWidget *child)
{
    if(!allowMultiple)
    {
        activateToggle(child->getName().c_str());
    }
    if(parent != NULL)
    {
        parent->triggerEvent(child);
    }
}

void ofxUILabelToggleMatrix::setAllowMultiple(bool _allowMultiple)
{
    allowMultiple = _allowMultiple;
}

void ofxUILabelToggleMatrix::mouseDragged(int x, int y, int button)
{
    if(hit)
    {
        bool tv = false;
        if(ofGetKeyPressed())
        {
            tv = true;
        }
        
        for(vector<ofxUILabelToggle *>::iterator it = toggles.begin(); it != toggles.end(); ++it)
        {
            if((*it)->isHit(x, y))
            {
                (*it)->setValue(tv);
            }
        }
    }
}

void ofxUILabelToggleMatrix::mousePressed(int x, int y, int button)
{
    if(rect->inside(x, y))
    {
        hit = true;
        state = OFX_UI_STATE_DOWN;
    }
    else
    {
        state = OFX_UI_STATE_NORMAL;
    }
    stateChange();
}

void ofxUILabelToggleMatrix::mouseReleased(int x, int y, int button)
{
    if(hit)
    {
        hit = false;
    }
}
