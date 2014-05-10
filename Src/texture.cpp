#include "tools.h"
#include "texture.h"

//#define DBG_TEXTURE_LOAD

unordered_map<string,Texture> Texture::_m;

void Texture::LoadTexture(string imgPath,string name) /// Laod and link texture
{
    m_name = name;
    m_path = imgPath;
    if(_m.count(name) == 0)
    {
#ifdef DBG_TEXTURE_LOAD
            cout << "Load -> " << imgPath;
#endif // DBG_TEXTURE_LOAD
        if(Tools::Settings::TextureLoadModern)
        {
            tex = SOIL_load_OGL_texture( imgPath.c_str() ,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
            if (tex == 0)
                cout << "SOIL loading error for -> " << imgPath << " :" <<  SOIL_last_result() << endl;

            _m[name] = (*this);
        }
        else
        {
            int width, height;
            unsigned char* image;

            glGenTextures( 1, &tex );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

            glGenerateMipmap(GL_TEXTURE_2D);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            image = SOIL_load_image( imgPath.c_str() , &width, &height, 0, SOIL_LOAD_RGB );
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, image);
            SOIL_free_image_data(image);

            _m[name] = (*this);
        }

#ifdef DBG_TEXTURE_LOAD
            cout << "Load -> " << imgPath;
#endif // DBG_TEXTURE_LOAD
    }
    else
        (*this) = _m[name];
}

void Texture::SetTexture( string name )
{
    if(_m.count(name) != 0)
        (*this) = _m[name];
}
