/*

  Author(s): Simon Leonard
  Created on: Dec 02 2009

  (C) Copyright 2009 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#ifndef _devCAN_h
#define _devCAN_h

#include <iostream>
#include <iomanip>

#include <cisstVector/vctDynamicVector.h>

//! Generic CAN bus
/**
   The only thing this class does is to define the interface that must be
   implemented by a CAN bus. Any implementation of a CAN device depends on an 
   actual device driver.
*/
class devCAN {

public:

  //! The different CAN rates 
  enum Rate { RATE_150 =150000, 
	      RATE_300 =300000, 
	      RATE_1000=1000000 };
  
  //! The different CAN rates 
  enum Errno { ESUCCESS, 
	       EFAILURE };
  
  enum Flags{ MSG_NOFLAG   = 0x00,
              MSG_CONFIRM  = 0x01,   // ask for a confirmation 
	      MSG_DONTWAIT = 0x02 }; // enables non-blocking operation

protected:

  //! The rate of the device
  devCAN::Rate rate;


public:

  //! A base frame of the CAN bus
  /**
     This defines a basic CAN frame. It does not support the extended frame 
     format. The frame is composed of an identifier, 8 bytes of data and a data 
     length. A CAN frame contain an 11 bits identifier, 0-8 bytes of data and a 
     data length code (0-8).
     This class does not support the extended frame format.
  */
  class Frame{
    
  public:
    
    //! The id of a CAN frame
    /**
       A CAN ID has 11 bits so we use 16 bits to represent a CAN id of which 
       only the 11 LSB are used.
    */
    typedef unsigned short ID;
    
    typedef unsigned short Mask;
    
    //! The data type
    typedef unsigned char Data;
    
    //! The field of data
    typedef Data DataField[8];
    
    //! The data field type
    typedef unsigned char DataLength;
    
  protected:
    
    //! The ID of the node origin
    Frame::ID id;
    
    //! The message (8 bytes)
    Frame::DataField data;
    
    //! The lenght of the message in bytes
    Frame::DataLength nbytes;
    
  public:
    
    
    //! Default constructor
    /**
       Set the IDs to "0" the message length to "0" and all the message bytes to
       "0"
    */
    Frame();
    
    //! Overloaded constructor
    /**
       Set the id of the CAN frame and the message
    */
    Frame( Frame::ID canid, Frame::DataField data, Frame::DataLength nbytes );

    Frame( Frame::ID canid, const vctDynamicVector<Frame::Data>& data );

    //! Return the identifier of the frame
    Frame::ID GetID() const { return id; }
  
    //! Return the length in bytes of the data
    Frame::DataLength GetLength() const { return nbytes; }

    //! Return a pointer to the data
    Frame::Data* GetData() { return  &(data[0]); }

    //! Return a const pointer to the data
    const Frame::Data* GetData() const { return  &(data[0]); }

    //! Output the can frame
    /**
       Prints out the frame's identifier, data length and data
       \param os[in] An ouptput stream
       \param cf[in] A CAN frame
    */
    friend std::ostream& operator<<( std::ostream& os, const Frame& frame ){
      os <<"ID: 0x" 
	 << std::hex << std::setfill('0') << std::setw(4) 
	 << (int)frame.GetID() << std::endl
	 << "Length: " << (int)frame.GetLength() << std::endl 
	 << "Data: ";
      for( Frame::DataLength i=0; i<frame.GetLength(); i++ )
	os << "0x" << std::hex << std::setfill('0') << std::setw(2) 
	   << (int)(frame.data[i]) << " ";
      os << std::dec;
      return os;
    }
    
  }; // Frame

  
public:
  
  // Filter

  class Filter {
    
  public:
    
    Frame::Mask mask;
    Frame::ID id;
    
    Filter( Frame::Mask mask, Frame::ID id ) : mask( mask ), id( id ) {}
    
  };  // Filter
  
public:
  
  //! Default constructor
  /**
     Initialize a CAN device with the given rate
     \param rate The CAN rate
  */
  devCAN( devCAN::Rate rate );

  //! Default destructor
  virtual ~devCAN();

  //! Open the CAN device
  virtual devCAN::Errno Open() = 0;

  //! Close the CAN device
  virtual devCAN::Errno Close() = 0;

  //! Send a CAN frame on the bus
  /**
     Call this method to send a CAN frame on the bus. The method can be blocking
     \param frame[in] The CAN frame to send on the bus
     \param block Block the device until the operation is completed
  */
  virtual devCAN::Errno Send( const devCAN::Frame& frame, 
			      devCAN::Flags flags = devCAN::MSG_NOFLAG ) = 0;

  //! Receive a CAN frame
  /**
     Call this method to receive a CAN frame. The method can be blocking.
     \param frame[out] The CAN frame received from the bus
     \param block Block the device until a CAN frame is received
  */
  virtual devCAN::Errno Recv( devCAN::Frame& frame, 
			      devCAN::Flags flags = devCAN::MSG_NOFLAG ) = 0;
  

  virtual devCAN::Errno AddFilter( const devCAN::Filter& filter ) = 0;

};


#endif // _devCAN_hpp