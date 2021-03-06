// -*- C++ -*-
/* GG is a GUI for SDL and OpenGL.
   Copyright (C) 2003 T. Zachary Laine

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA

   If you do not wish to comply with the terms of the LGPL please
   contact the author as other terms are available for a fee.
    
   Zach Laine
   whatwasthataddress@hotmail.com */

/** \file Texture.h
    Contains the Texture class, which encapsulates an OpenGL texture object; the SubTexture class, which represents a portion 
    of an OpenGL texture object; and the TextureManager class, which provides GUI-wide management of Texture objects. */

#ifndef _GG_Texture_h_
#define _GG_Texture_h_

#include <GG/Base.h>
#include <GG/Exception.h>

#include <boost/serialization/access.hpp>
#include <boost/serialization/binary_object.hpp>


namespace GG {

/** This class encapsulates OpenGL texture objects.  If the dimensions of the image used to initialize the texture are
    not both powers of two, the texture is created with dimensions of the next largest (or equal) powers of two.  The
    original image occupies the region near the texture's origin, and the rest is zero-initialized.  This is done to
    prevent the image from being scaled, since textures used in a GUI almost always must maintain pixel accuracy.  The
    original image size and corresponding texture coords are saved, and can be accessed through DefaultWidth(),
    DefaultHeight(), and DefaultTexCoords(), respectively.  These are kept so that only the originally-loaded-image part
    of the texture can be used, if desired.  All initialization functions first free the OpenGL texture currently in use
    by the texture (if any) and create a new one.  When the load filename is "" or the image parameter is 0, all
    initialization functions fail silently, performing no initialization and allocating no memory or OpenGL texture.
    Serialized Textures save the filename associated with the texture when available, so the originally loaded file can
    be reloaded again later.  If no such file exists, such as when a Texture is created from in-memory image data, the
    contents of the Texture are read from video memory and saved as binary data.  A default-constructed Texture will
    have niether a filename nor raw image data. */
class GG_API Texture
{
public:
    /** \name Structors */ ///@{
    Texture();          ///< ctor
    virtual ~Texture(); ///< virtual dtor
    //@}

    /** \name Accessors */ ///@{
    std::string      Filename() const;         ///< returns the filename from which this texture was loaded ("" if this texture was not loaded from a file)
    GLenum           WrapS() const;            ///< returns S-wrap mode associated with this opengl texture
    GLenum           WrapT() const;            ///< returns T-wrap mode associated with this opengl texture
    GLenum           MinFilter() const;        ///< returns minimization filter modes associated with this opengl texture
    GLenum           MagFilter() const;        ///< returns maximization filter modes associated with this opengl texture
    int              BytesPP() const;          ///< returns the image's color depth in bytes
    GLint            Width() const;            ///< returns width of entire texture
    GLint            Height() const;           ///< returns height of entire texture
    bool             MipMapped() const;        ///< returns true if the texture has mipmaps
    GLuint           OpenGLId() const;         ///< GLuint "name" of the opengl texture object associated with this object
    const GLfloat*   DefaultTexCoords() const; ///< texture coordinates to use by default when blitting this texture
    GLint            DefaultWidth() const;     ///< returns width in pixels, based on initial image (0 if texture was not loaded)
    GLint            DefaultHeight() const;    ///< returns height in pixels, based on initial image (0 if texture was not loaded)

    /** blit any portion of texture to any place on screen, scaling as necessary*/
    void OrthoBlit(const Pt& pt1, const Pt& pt2, const GLfloat* tex_coords = 0) const;

    /** blit default portion of texture unscaled to \a pt (upper left corner)*/
    void OrthoBlit(const Pt& pt) const;
    //@}

    /** \name Mutators */ ///@{
    // intialization functions
    /** frees any currently-held memory and loads a texture from file \a filename.  \throw GG::Texture::BadFile Throws
        if the texture creation fails. */
    void Load(const std::string& filename, bool mipmap = false);

    /** frees any currently-held memory and creates a texture from supplied array \a image.  \throw
        GG::Texture::Exception Throws applicable subclass if the texture creation fails in one of the specified ways. */
    void Init(int width, int height, const unsigned char* image, GLenum format, GLenum type, int bytes_per_pixel, bool mipmap = false);

    /** frees any currently-held memory and creates a texture from subarea of supplied array \a image.  \throw
        GG::Texture::Exception Throws applicable subclass if the texture creation fails in one of the specified ways. */
    void Init(int x, int y, int width, int height, int image_width, const unsigned char* image, GLenum format, GLenum type, int bytes_per_pixel, bool mipmap = false);

    void SetWrap(GLenum s, GLenum t);         ///< sets the opengl texture wrap modes associated with opengl texture m_opengl_id
    void SetFilters(GLenum min, GLenum mag);  ///< sets the opengl min/mag filter modes associated with opengl texture m_opengl_id
    void Clear();  ///< frees the opengl texture object associated with this object
    //@}

    /** \name Exceptions */ ///@{
    /** The base class for Texture exceptions. */
    GG_ABSTRACT_EXCEPTION(Exception);

    /** Thrown when valid texture data cannot be read from a file. */
    GG_CONCRETE_EXCEPTION(BadFile, GG::Texture, Exception);

    /** Thrown when an unsupported number of color channels is requested. */
    GG_CONCRETE_EXCEPTION(InvalidColorChannels, GG::Texture, Exception);

    /** Thrown when GL fails to provide a requested texture object. */
    GG_CONCRETE_EXCEPTION(InsufficientResources, GG::Texture, Exception);
    //@}

private:
    Texture(const Texture& rhs);             ///< disabled
    Texture& operator=(const Texture& rhs);  ///< disabled
    void InitFromRawData(int width, int height, const unsigned char* image, GLenum format, GLenum type, int bytes_per_pixel, bool mipmap);
    unsigned char* GetRawBytes();

    std::string m_filename;   ///< filename from which this Texture was constructed ("" if not loaded from a file)

    int         m_bytes_pp;
    GLint       m_width;
    GLint       m_height;

    GLenum      m_wrap_s, m_wrap_t;
    GLenum      m_min_filter, m_mag_filter;

    bool        m_mipmaps;
    GLuint      m_opengl_id;   ///< OpenGL texture ID
    GLenum      m_format;
    GLenum      m_type;

    /// each of these is used for a non-power-of-two-sized graphic loaded into a power-of-two-sized texture
    GLfloat     m_tex_coords[4];  ///< the texture coords used to blit from this texture by default (reflecting original image width and height)
    GLint       m_default_width;  ///< the original width and height of this texture to be used in blitting 
    GLint       m_default_height;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

/** This class is a convenient way to store the info needed to use a portion of an OpenGL texture. */
class GG_API SubTexture
{
public:
    /** \name Structors */ ///@{
    SubTexture(); ///< default ctor

    /** creates a SubTexture from a GG::Texture and coordinates into it.  \throw GG::SubTexture::BadTexture Throws if
        the given Texture is null.  \throw GG::SubTexture::InvalidTextureCoordinates Throws if the texture coordinates
        are not well formed.*/
    SubTexture(const boost::shared_ptr<const Texture>& texture, int x1, int y1, int x2, int y2);  

    SubTexture(const SubTexture& rhs); ///< copy ctor
    const SubTexture& operator=(const SubTexture& rhs); ///< assignment operator
    virtual ~SubTexture(); ///< virtual dtor
    //@}

    /** \name Accessors */ ///@{
    bool             Empty() const;     ///< returns true if this object has no associated GG::Texture
    const GLfloat*   TexCoords() const; ///< texture coordinates to use when blitting this sub-texture
    GLint            Width() const;     ///< width of sub-texture in pixels
    GLint            Height() const;    ///< height of sub-texture in pixels
    const Texture*   GetTexture() const;///< returns the texture the SubTexture is a part of

    /** blit sub-texture to any place on screen, scaling as necessary \see GG::Texture::OrthoBlit*/
    void OrthoBlit(const Pt& pt1, const Pt& pt2) const;

    /** blit sub-texture unscaled to \a pt (upper left corner) \see GG::Texture::OrthoBlit*/
    void OrthoBlit(const Pt& pt) const;
    //@}

    /** \name Exceptions */ ///@{
    /** The base class for SubTexture exceptions. */
    GG_ABSTRACT_EXCEPTION(Exception);

    /** Thrown when an attempt is made to create a SubTexture using a null texture. */
    GG_CONCRETE_EXCEPTION(BadTexture, GG::SubTexture, Exception);

    /** Thrown when invalid or out-of-order texture coordinates are supplied.*/
    GG_CONCRETE_EXCEPTION(InvalidTextureCoordinates, GG::SubTexture, Exception);
    //@}

private:
    boost::shared_ptr<const Texture> m_texture;        ///< shared_ptr to texture object with entire image
    GLint                            m_width;
    GLint                            m_height;
    GLfloat                          m_tex_coords[4];  ///< position of element within containing texture 

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

/** This singleton class is essentially a very thin wrapper around a map of Texture smart pointers, keyed on std::string
    texture names.  The user need only request a texture through GetTexture(); if the texture is not already resident,
    it will be loaded.  If the user would like to create her own images and store them in the manager, that can be
    accomplished via StoreTexture() calls.*/
class GG_API TextureManager
{
public:
    /** \name Mutators */ ///@{
    /** stores a pre-existing GG::Texture in the manager's texture pool, and returns a shared_ptr to it. \warning Calling code <b>must not</b> 
        delete \a texture; \a texture becomes the property of the manager, which will eventually delete it. */
    boost::shared_ptr<Texture> StoreTexture(Texture* texture, const std::string& texture_name);

    /** stores a pre-existing GG::Texture in the manager's texture pool, and returns a shared_ptr to it. \warning Calling code <b>must not</b> 
        delete \a texture; \a texture becomes the property of the manager, which will eventually delete it. */
    boost::shared_ptr<Texture> StoreTexture(const boost::shared_ptr<Texture>& texture, const std::string& texture_name);

    /** returns a shared_ptr to the texture created from image file \a name.  If the texture is not present in the
        manager's pool, it will be loaded from disk. */
    boost::shared_ptr<Texture> GetTexture(const std::string& name, bool mipmap = false);

    /** removes the manager's shared_ptr to the texture created from image file \a name, if it exists.  \note Due to shared_ptr semantics, 
        the texture may not be deleted until much later. */
    void                FreeTexture(const std::string& name);
    //@}

    static void         InitDevIL(); ///< initializes DevIL image library, if it is not already initialized

private:
    TextureManager();
    boost::shared_ptr<Texture> LoadTexture(const std::string& filename, bool mipmap);

    static bool s_created;
    static bool s_il_initialized;
    std::map<std::string, boost::shared_ptr<Texture> > m_textures;

    friend TextureManager& GetTextureManager();
};

/** Returns the singleton TextureManager instance. */
TextureManager& GetTextureManager();

} // namespace GG

// template implementations
template <class Archive>
void GG::Texture::serialize(Archive& ar, const unsigned int version)
{
    ar  & BOOST_SERIALIZATION_NVP(m_filename)
        & BOOST_SERIALIZATION_NVP(m_bytes_pp)
        & BOOST_SERIALIZATION_NVP(m_width)
        & BOOST_SERIALIZATION_NVP(m_height)
        & BOOST_SERIALIZATION_NVP(m_wrap_s)
        & BOOST_SERIALIZATION_NVP(m_wrap_t)
        & BOOST_SERIALIZATION_NVP(m_min_filter)
        & BOOST_SERIALIZATION_NVP(m_mag_filter)
        & BOOST_SERIALIZATION_NVP(m_mipmaps)
        & BOOST_SERIALIZATION_NVP(m_format)
        & BOOST_SERIALIZATION_NVP(m_type)
        & BOOST_SERIALIZATION_NVP(m_tex_coords)
        & BOOST_SERIALIZATION_NVP(m_default_width)
        & BOOST_SERIALIZATION_NVP(m_default_height);

    unsigned char* raw_data_bytes = 0;
    std::size_t raw_data_size = 0;
    if (Archive::is_saving::value) {
        if (m_filename == "" && m_opengl_id)
            raw_data_bytes = GetRawBytes();
        if (raw_data_bytes)
            raw_data_size = static_cast<std::size_t>(m_width * m_height * m_bytes_pp);
    }
    ar & BOOST_SERIALIZATION_NVP(raw_data_size);
    if (raw_data_size) {
        if (Archive::is_loading::value) {
            typedef unsigned char uchar;
            raw_data_bytes = new uchar[raw_data_size];
        }
        boost::serialization::binary_object raw_data(raw_data_bytes, raw_data_size);
        ar & BOOST_SERIALIZATION_NVP(raw_data);
    }

    if (Archive::is_loading::value) {
        if (raw_data_size) {
            Init(0, 0, m_default_width, m_default_height, m_width, raw_data_bytes, m_format, m_type, m_bytes_pp, m_mipmaps);
        } else {
            try {
                Load(m_filename);
            } catch (const BadFile& e) {
                // take no action; the Texture must have been uninitialized when saved
            }
        }
        SetWrap(m_wrap_s, m_wrap_t);
        SetFilters(m_min_filter, m_mag_filter);
    }

    if (raw_data_size)
        delete [] raw_data_bytes;
}

template <class Archive>
void GG::SubTexture::serialize(Archive& ar, const unsigned int version)
{
    boost::shared_ptr<Texture> non_const_texture;
    if (Archive::is_saving::value)
        non_const_texture = boost::const_pointer_cast<Texture>(m_texture);

    ar  & boost::serialization::make_nvp("m_texture", non_const_texture)
        & BOOST_SERIALIZATION_NVP(m_width)
        & BOOST_SERIALIZATION_NVP(m_height)
        & BOOST_SERIALIZATION_NVP(m_tex_coords);

    if (Archive::is_loading::value)
        m_texture = boost::const_pointer_cast<const Texture>(non_const_texture);
}

#endif // _GG_Texture_h_
