// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Copyright (C) 2014 Henner Zeller <h.zeller@acm.org>
// Copyright (C) 2015 Christoph Friedrich <christoph.friedrich@vonaffenfels.de>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://gnu.org/licenses/gpl-2.0.txt>

#include <assert.h>
#include <stdio.h>

#include "transformer.h"

namespace rgb_matrix {
/********************************/
/* Snake 8x2 Transformer Canvas */
/********************************/
class Snake8x2Transformer::TransformCanvas : public Canvas {
public:
  TransformCanvas() : delegatee_(NULL) {}

  void SetDelegatee(Canvas* delegatee);

  virtual void Clear();
  virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);
  virtual int width() const;
  virtual int height() const;
  virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);

private:
  Canvas *delegatee_;
};

void Snake8x2Transformer::TransformCanvas::SetDelegatee(Canvas* delegatee) {
  delegatee_ = delegatee;
}

void Snake8x2Transformer::TransformCanvas::Clear() {
  delegatee_->Clear();
}

void Snake8x2Transformer::TransformCanvas::Fill(uint8_t red, uint8_t green, uint8_t blue) {
  delegatee_->Fill(red, green, blue);
}

int Snake8x2Transformer::TransformCanvas::width() const {
  return delegatee_->width() / 2;
}

int Snake8x2Transformer::TransformCanvas::height() const {
  return delegatee_->height() * 2;
}

void Snake8x2Transformer::TransformCanvas::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
  int new_x;
  int new_y;

  new_x = ((x / 8) * 16) + (x % 8);
  if ((y & 4) == 0) {
    new_x += 8;
  }

  new_y = ((y / 8) * 4) + (y % 4);

  delegatee_->SetPixel(new_x, new_y, red, green, blue);
}

 /********************************/
/* My New Transformer Canvas */
/********************************/
class MyNewTransformer::TransformCanvas : public Canvas {
public:
  TransformCanvas() : delegatee_(NULL) {}

  void SetDelegatee(Canvas* delegatee);

  virtual void Clear();
  virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);
  virtual int width() const;
  virtual int height() const;
  virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);

private:
  Canvas *delegatee_;
};

void MyNewTransformer::TransformCanvas::SetDelegatee(Canvas* delegatee) {
  delegatee_ = delegatee;
}

void MyNewTransformer::TransformCanvas::Clear() {
  delegatee_->Clear();
}

void MyNewTransformer::TransformCanvas::Fill(uint8_t red, uint8_t green, uint8_t blue) {
  delegatee_->Fill(red, green, blue) /* add any necessary transform of color here */;
}

int MyNewTransformer::TransformCanvas::width() const {
  return delegatee_->width() / 2 /* add any necessary transform of width here */;
}

int MyNewTransformer::TransformCanvas::height() const {
  return delegatee_->height()/* add any necessary transform of height here */;
}

void MyNewTransformer::TransformCanvas::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
  int new_x = x;
  int new_y = y;

  // printf("\nGot x:y as %i:%i\n", x, y);
  
  // Verified that the transformer is active
  // new_y = 2;
  // new_x = 2; 
  
  new_x = x + (((x/32) + (1 - ((y/8)%2))) * 32);
  // if (((y/8)%2) == 0) {
  //   new_x = x + (((x/32)+1)*32);
  //   // if (x >= 0 && x < 32) {
  //   //   new_x = x + 32;
  //   // } else if (x >= 32 && x < 64) {
  //   //   new_x = x + 64;
  //   // } else if (x >=64 && x < 96) {
  //   //   new_x = x + 96;
  //   // } else if (x >= 96 && x < 128) {
  //   //   new_x = x + 128;
  //   // }
  // } else {
  //   new_x = x + ((x/32)*32);
  //   // if (x >= 0 && x < 32) {
  //   //   new_x = x;
  //   // } else if (x >= 32 && x < 64) {
  //   //   new_x = x + 32;
  //   // } else if (x >= 64 && x < 96) {
  //   //   new_x = x + 64;
  //   // } else if (x >= 96 && x < 128) {
  //   //   new_x = x + 96;
  //   // }
  // }

  // printf("\nNew pixels: %i, %i :: %i,%i,%i\n", new_x, new_y, red, green, blue);
  delegatee_->SetPixel(new_x, new_y, red, green, blue);
}

MyNewTransformer::MyNewTransformer()
  : canvas_(new TransformCanvas()) {
}

MyNewTransformer::~MyNewTransformer() {
  delete canvas_;
}

Canvas *MyNewTransformer::Transform(Canvas *output) {
  assert(output != NULL);

  canvas_->SetDelegatee(output);
  return canvas_;
}

/***********************************************************************************************/

/*****************************/
/* Rotate Transformer Canvas */
/*****************************/
class RotateTransformer::TransformCanvas : public Canvas {
public:
  TransformCanvas(int angle);

  void SetDelegatee(Canvas* delegatee);
  void SetAngle(int angle);

  virtual int width() const;
  virtual int height() const;
  virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
  virtual void Clear();
  virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);

private:
  Canvas *delegatee_;
  int angle_;
};

RotateTransformer::TransformCanvas::TransformCanvas(int angle)
  : delegatee_(NULL) {
  SetAngle(angle);
}

void RotateTransformer::TransformCanvas::SetDelegatee(Canvas* delegatee) {
  delegatee_ = delegatee;
}

void RotateTransformer::TransformCanvas::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
  switch (angle_) {
  case 0:
    delegatee_->SetPixel(x, y, red, green, blue);
    break;
  case 90:
    delegatee_->SetPixel(delegatee_->width() - y - 1, x,
                         red, green, blue);
    break;
  case 180:
    delegatee_->SetPixel(delegatee_->width() - x - 1,
                         delegatee_->height() - y - 1,
                         red, green, blue);
    break;
  case 270:
    delegatee_->SetPixel(y, delegatee_->height() - x - 1, red, green, blue);
    break;
  }
}

int RotateTransformer::TransformCanvas::width() const {
  return (angle_ % 180 == 0) ? delegatee_->width() : delegatee_->height();
}

int RotateTransformer::TransformCanvas::height() const {
  return (angle_ % 180 == 0) ? delegatee_->height() : delegatee_->width();
}

void RotateTransformer::TransformCanvas::Clear() {
  delegatee_->Clear();
}

void RotateTransformer::TransformCanvas::Fill(uint8_t red, uint8_t green, uint8_t blue) {
  delegatee_->Fill(red, green, blue);
}

void RotateTransformer::TransformCanvas::SetAngle(int angle) {
  assert(angle % 90 == 0);  // We currenlty enforce that for more pretty output
  angle_ = (angle + 360) % 360;
}

/**********************/
/* Rotate Transformer */
/**********************/
RotateTransformer::RotateTransformer(int angle)
  : angle_(angle), canvas_(new TransformCanvas(angle)) {
}

RotateTransformer::~RotateTransformer() {
  delete canvas_;
}

Canvas *RotateTransformer::Transform(Canvas *output) {
  assert(output != NULL);

  canvas_->SetDelegatee(output);
  return canvas_;
}

void RotateTransformer::SetAngle(int angle) {
  canvas_->SetAngle(angle);
  angle_ = angle;
}

/**********************/
/* Linked Transformer */
/**********************/
void LinkedTransformer::AddTransformer(CanvasTransformer *transformer) {
  list_.push_back(transformer);
}

void LinkedTransformer::AddTransformer(List transformer_list) {
  list_.insert(list_.end(), transformer_list.begin(), transformer_list.end());
}
void LinkedTransformer::SetTransformer(List transformer_list) {
  list_ = transformer_list;
}

Canvas *LinkedTransformer::Transform(Canvas *output) {
  for (size_t i = 0; i < list_.size(); ++i) {
    output = list_[i]->Transform(output);
  }

  return output;
}

void LinkedTransformer::DeleteTransformers() {
  for (size_t i = 0; i < list_.size(); ++i) {
    delete list_[i];
  }
  list_.clear();
}

// U-Arrangement Transformer.
class UArrangementTransformer::TransformCanvas : public Canvas {
public:
  TransformCanvas(int parallel) : parallel_(parallel), delegatee_(NULL) {}

  void SetDelegatee(Canvas* delegatee);

  virtual void Clear();
  virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);
  virtual int width() const { return width_; }
  virtual int height() const { return height_; }
  virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);

private:
  const int parallel_;
  int width_;
  int height_;
  int panel_height_;
  Canvas *delegatee_;
};

void UArrangementTransformer::TransformCanvas::SetDelegatee(Canvas* delegatee) {
  delegatee_ = delegatee;
  width_ = (delegatee->width() / 64) * 32;   // Div in middle at 32px boundary
  height_ = 2 * delegatee->height();
  if (delegatee->width() % 64 != 0) {
    fprintf(stderr, "An U-arrangement would need an even number of panels "
            "unless you can fold one in the middle...\n");
  }
  if (delegatee->height() % parallel_ != 0) {
    fprintf(stderr, "For parallel=%d we would expect the height=%d to be "
            "divisible by %d ??\n", parallel_, delegatee->height(), parallel_);
    assert(false);
  }
  panel_height_ = delegatee->height() / parallel_;
}

void UArrangementTransformer::TransformCanvas::Clear() {
  delegatee_->Clear();
}

void UArrangementTransformer::TransformCanvas::Fill(
  uint8_t red, uint8_t green, uint8_t blue) {
  delegatee_->Fill(red, green, blue);
}

void UArrangementTransformer::TransformCanvas::SetPixel(
  int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
  if (x < 0 || x >= width_ || y < 0 || y >= height_) return;
  const int slab_height = 2*panel_height_;   // one folded u-shape
  const int base_y = (y / slab_height) * panel_height_;
  y %= slab_height;
  if (y < panel_height_) {
    x += delegatee_->width() / 2;
  } else {
    x = width_ - x - 1;
    y = slab_height - y - 1;
  }
  delegatee_->SetPixel(x, base_y + y, red, green, blue);
}

UArrangementTransformer::UArrangementTransformer(int parallel)
  : canvas_(new TransformCanvas(parallel)) {
  assert(parallel > 0);
}

UArrangementTransformer::~UArrangementTransformer() {
  delete canvas_;
}

Canvas *UArrangementTransformer::Transform(Canvas *output) {
  assert(output != NULL);

  canvas_->SetDelegatee(output);
  return canvas_;
}

// Legacly LargeSquare64x64Transformer: uses the UArrangementTransformer, but
// does things so that it looks the same as before.
LargeSquare64x64Transformer::LargeSquare64x64Transformer()
  : arrange_(1), rotated_(180) { }
Canvas *LargeSquare64x64Transformer::Transform(Canvas *output) {
  return rotated_.Transform(arrange_.Transform(output));
}
} // namespace rgb_matrix