/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "DataSaver.h"

DataSaver::DataSaver()
{
}

// Opens the file
bool DataSaver::InitDataSaving(const QString &filename, bool isBnry)
{
    if( !os.good() || os.is_open() ) {
        os.close();
        os.clear();
    }

    isBinary = isBnry;
    if( isBinary == 0 ) os.open(filename.toLatin1());  // ascii write
    else                os.open(filename.toLatin1(), std::ios::out | std::ios::binary);    // binary write

    return os.good() && os.is_open();
}


// Consume data pushed to the queues by DCThread on the main thread
void DataSaver::SaveLine()
{
    float data;
    if ( isBinary ) {
        for ( auto& queue : q ) {
            queue->pop(data);
            os.write(reinterpret_cast<char*>(&data), sizeof data);
        }
    } else {
        for ( auto& queue : q ) {
            queue->pop(data);
            os << data << " ";
        }
        os << "\n";
    }
    os.flush();
}


// Saves the header into the file
void DataSaver::SaveHeader(QVector<QString> header, double savingFreq)
{

  if( isBinary == 0 ) {     // Ascii write

      os << "% ";

      for ( int i= 0; i < header.size(); i++ ){
          for ( int j= 0; j<header[i].size(); j++ )
              os << header[i][j].toLatin1();
          os << " ";

      }
      os << "\n";

  }
  else {                    // Binary write

      float data;
      for ( int i= 0; i < header.size(); i++ ) {
          for ( int j= 0; j<header[i].size(); j++ ) {
              data = (char) header[i][j].toLatin1();
              os.write( reinterpret_cast<char*>( &data ), sizeof data );
          }
      }

  }

  // flush the buffer
  os.flush();

  // prepare queues
  size_t bufsz (savingFreq > 1e3 ? savingFreq : 1e3);
  q.clear();
  for ( int i = 0; i < header.size(); i++ )
      q.push_back(std::unique_ptr<CircularFifo<float>>(new CircularFifo<float>(bufsz)));
}


// Closes the file
void DataSaver::EndDataSaving()
{
    // Not a race condition: This slot is called (via a QueuedConnection) on the main thread,
    // so no call to SaveLine() should remain.
    os.close();
    os.clear();
}
