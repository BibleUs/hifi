//
//  TGAReader.h
//  image/src/image
//
//  Created by Ryan Huffman
//  Copyright 2019 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_image_TGAReader_h
#define hifi_image_TGAReader_h

#include <QImage>

namespace image {

// TODO Move this into a plugin that QImageReader can use
QImage readTGA(QIODevice& contents);

}

#endif // hifi_image_TGAReader_h
