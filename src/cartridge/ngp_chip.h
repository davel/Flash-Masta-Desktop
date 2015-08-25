/*! \file
 *  \brief File containing the declaration of the \ref ngp_chip class.
 *  
 *  file containing the header information and declaration of the
 *  \ref ngp_chip class.
 *  
 *  \author Daniel Andrus
 *  \date 2015-07-29
 *  \copyright Copyright (c) 2015 7400 Circuits. All rights reserved.
 */

#ifndef __NGP_CHIP_H__
#define __NGP_CHIP_H__

class linkmasta_device;
class task_controller;

/*! \class ngp_chip
 *  \brief Class for controlling and interacting with a flash storage chip on a
 *         Neo Geo Pocket cartridge.
 *  
 *  Class for controlling and interacting with a flash storage chip on a Neo Geo
 *  Pocket cartridge. Contains logic for manipulating the device's current mode,
 *  erasing sectors on the device, programming the device, and getting device
 *  information.
 *  
 *  This class is intended to be used as a communication utility
 *  and stores very little information about the current state of the device
 *  that it represents.
 *  
 *  This class is *not* thread-safe. use caution when working in a multithreaded
 *  environment.
 */
class ngp_chip
{
public:
  /*! \brief Type used for data content. */
  typedef unsigned char    data_t;
  
  /*! \brief Type used for sending commands and reading words from the chip. */
  typedef unsigned char    word_t;
  
  /*!
   * \brief Type used for specifying the index of the device on the cartridge.
   */
  typedef unsigned int     chip_index_t;
  
  /*! \brief Type used for the device's manufacturer id. */
  typedef unsigned int     manufact_id_t;
  
  /*! \brief Type used for the device's device id. */
  typedef unsigned int     device_id_t;
  
  /*!
   *  \brief Type used for indicating whether a sector on the device is
   *         write protected.
   */
  typedef bool             protect_t;
  
  /*! \brief Type used for specifying an address an on the device. */
  typedef unsigned int     address_t;
  
  /*! \enum chip_mode
   *  \brief Enumeration representing the assumed state of the device.
   *  
   *  Enumeration representing the current assumed state of the device.
   *  
   *  \see current_mode()
   */
  enum chip_mode
  {
    /*! \brief Device is in the default read mode. */
    READ,
    
    /*! \brief Device is in autoselect mode, ready for metadata queries. */
    AUTOSELECT,
    
    /*! \brief Device is in bypass mode, ready for swift programming. */
    BYPASS,
    
    /*! \brief Device is busy erasing. */
    ERASE
  };
  
  /*! \brief The constructor for this class.
   *  
   *  The main constructor for the class. Initializes members with supplied
   *  parameters.
   *  
   *  \param linkmasta_device Reference to the linkmasta device this chip is to
   *         use for communication with the hardware. Must be a pointer to a
   *         valid object in memory.
   *  \param [in] chip_num This device's index on the cartridge.
   *  
   *  \see linkmasta_device
   */
                          ngp_chip(linkmasta_device* linkmasta_device, chip_index_t chip_num);
  
  /*! \brief The destructor for the class.
   *  
   *  Frees dynamically allocated memory and closes any open connections.
   *  
   *  To prevent the device being in an inconsistent state, be sure to call
   *  \ref reset() before allowing this object to be destroyed.
   *  
   *  \see reset()
   */
                          ~ngp_chip();
  
  
  
  /*! \brief Reads a single word from the chip.
   *  
   *  Reads a single word from the chip from the provided address. The exact
   *  result of this method can vary depending on the current state of the chip.
   *  For example, if the chip is in \ref chip_mode::READ mode, then the device
   *  should give the data stored at the given address. If the device is in
   *  \ref chip_mode::AUTOSELECT, then this method can be used to request
   *  specific metadata about the chip. See the device's specification sheet to
   *  learn what commands are valid.
   *  
   *  \param [in] address The address from which to request a word.
   *  
   *  \returns The word returned from the device.
   *  
   *  \see address_t
   *  \see chip_mode
   *  \see current_mode()
   */
  word_t                  read(address_t address);
  
  /*! \brief Writes a single word to the chip.
   *  
   *  Writes a single word from the chip. This does not program the data in the
   *  chip's storage, but instead can be used to send commands directly to the
   *  device. See the device's specification sheet to learn what commands are
   *  valid.
   *  
   *  \param [in] address The address to which to write.
   *  \param [in] data The word of data to send to the chip.
   *  
   *  \see program_byte(address_t address, data_t data);
   */
  void                    write(address_t address, word_t data);
  
  
  
  /*! \brief Commands the device to enter the default \ref chip_mode::READ mode.
   *  
   *  Sends the reset command sequence to the hardware device. Whether or not
   *  the operation is successful is not guaranteed.
   *  
   *  This function is a blocking function that can take several seconds to
   *  complete.
   *  
   *  Causes the device to enter \ref chip_mode::READ mode.
   *  
   *  \see chip_mode::READ
   *  \see current_mode()
   */
  void                    reset();
  
  /*! \brief Commands the device to fetch the manufacturer id.
   *  
   *  Sends the command sequence necessary to enter \ref chip_mode::AUTOSELECT
   *  mode before requesting the manufacturer's id. The success of this
   *  operation is not guaranteed.
   *  
   *  This function is a blocking function that can take several seconds to
   *  complete.
   *  
   *  Causes the device to enter \ref chip_mode::AUTOSELECT mode.
   *  
   *  \returns If the operation executes as expected, will return the
   *           manufacturer's id. Otherwise, the result is undefined.
   *  
   *  \see chip_mode::AUTOSELECT
   *  \see current_mode()
   */
  manufact_id_t           get_manufacturer_id();
  
  /*! \brief Commands the device to fetch the device id.
   *  
   *  Sends the command sequence necessary to enter \ref chip_mode::AUTOSELECT
   *  mode before requesting the device id. The success of this operation is not
   *  guaranteed.
   *  
   *  This function is a blocking function that can take several seconds to
   *  complete.
   *  
   *  Causes the device to enter \ref chip_mode::AUTOSELECT mode.
   *  
   *  \returns If the operation executes as expected, will return the device id.
   *           Otherwise, the result is undefined.
   *  
   *  \see chip_mode::AUTOSELECT
   *  \see current_mode()
   */
  device_id_t             get_device_id();
  
  /*! \brief Queries the device on the protection status of a specific sector.
   *  
   *  Sends the command sequence necessary to enter \ref chip_mode::AUTOSELECT
   *  mode before requesting the protection status of a sector.
   *  
   *  This function is a blocking function that can take several seconds to
   *  complete.
   *  
   *  Causes the device to enter \ref chip_mode::AUTOSELECT mode.
   *  
   *  \param [in] sector_address The base address of the block (sector) to
   *         test for protection.
   *  
   *  \returns If successful, returns **true** if the sector is proteccted and
   *           **false** if it is unprotected.
   *  
   *  \see chip_mode::AUTOSELECT
   *  \see current_mode()
   */
  protect_t               get_block_protection(address_t sector_address);
  
  /*! \brief Attempts to program a word at a specific address on the chip.
   *  
   *  Attepts to program a word at a specific address on the chip. See note
   *  below on how to use this functionality when working with flash-based
   *  storage.
   *  
   *  This function is a blocking function that can take several seconds to
   *  complete.
   *  
   *  Causes the device to enter \ref chip_mode::READ mode.
   *  
   *  \note
   *  This function must be used with knowledge about how flash storage chips
   *  work. Writing to flash storage can only reset bits. That means that only
   *  **0**'s can be written to flash storage. In order to write **1**'s to
   *  flash storage, the entire sector containing the destination address must
   *  be erased.
   *  
   *  \par
   *  Before calling this function, ensure that the data at the destination
   *  address has been erased.
   *  
   *  \param [in] address The address on the chip to program.
   *  \param [in] data The data to write to flash storage.
   */
  void                    program_byte(address_t address, data_t data);
  
  /*! \brief Attempts to cause the device to enter bypass mode.
   *  
   *  Sends the command sequence to the device that will cause it to enter
   *  \ref chip_mode::BYPASS mode. When in \ref chip_mode::BYPASS mode, the
   *  device can program data much faster. See note below on how to use this
   *  functionality.
   *  
   *  This function is a blocking function that can take several seconds to
   *  complete.
   *  
   *  Causes the device to enter \ref chip_mode::BYPASS mode.
   *  
   *  \note
   *  This functionality is only available on certain devices. If this object
   *  determines that the current device does not support bypass mode, then this
   *  funtion will do nothing.
   */
  void                    unlock_bypass();
  
  /*! \brief Sends the command sequence to erase all data in the chip's flash
   *         storage.
   *  
   *  Sends the command sequence to erase all data in the chip's flash storage,
   *  setting all bits to **1**. See note below on how to use this
   *  functionality.
   *  
   *  \note
   *  Once this method has been called, it is the
   *  responsibility of the caller to poll the chip until the erase operation
   *  is complete. To test if the chip is erasing, the caller must call the
   *  \ref test_erasing() function. *Calls to \ref is_erasing() or testing the
   *  device's current mode with \ref current_mode() will not work!*
   *  
   *  Causes the device to enter \ref chip_mode::ERASE.
   */
  void                    erase_chip();
  
  /*! \brief Sends the command sequence to erase a single sector in the chip's
   *         flash storage.
   *  
   *  Sends the command sequence to erase an individual sector in the chip's
   *  flash storage, setting all bits within to **1**. See note below on how to
   *  use this functionality.
   *  
   *  \note
   *  Once this method has been called, it is the
   *  responsibility of the caller to poll the chip until the erase operation
   *  is complete. To test if the chip is erasing, the caller must call the
   *  \ref test_erasing() function. *Calls to \ref is_erasing() or testing the
   *  device's current mode with \ref current_mode() will not work!*
   *  
   *  Causes the device to enter \ref chip_mode::ERASE.
   *  
   *  \param [in] block_address The base address of the sector to erase. If this
   *         address is not the exact base address of a valid sector, then the
   *         erase operation will likely never initialize or will never
   *         complete.
   */
  void                    erase_block(address_t block_address);
  
  chip_mode               current_mode() const;
  bool                    supports_bypass() const;
  bool                    test_bypass_support();
  bool                    is_erasing() const;
  bool                    test_erasing();
  unsigned int            read_bytes(address_t address, data_t* data, unsigned int num_bytes, task_controller* controller = nullptr);
  unsigned int            program_bytes(address_t address, const data_t* data, unsigned int num_bytes, task_controller* controller = nullptr);
  
private:
  void                    enter_autoselect();
  
  chip_mode               m_mode;
  address_t               m_last_erased_addr;
  
  bool                    m_supports_bypass;
  
  linkmasta_device* const m_linkmasta;
  chip_index_t const      m_chip_num;
};

#endif /* defined(__NGP_CHIP_H__) */
