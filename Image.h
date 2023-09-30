#pragma once
#include "AppStructure.h"
#include<functional>
#include<vector>
#include<string>
#include<opencv2/opencv.hpp>
#include <GL/glcorearb.h>
namespace Window
{
	class ImageItem : public AppStructure
	{
	public:
		static std::shared_ptr<AppStructure> Create(std::string name, bool sameLine, int width, int height)
		{
			std::shared_ptr<AppStructure> node = std::shared_ptr<ImageItem>(new ImageItem(name, sameLine, width, height), [](ImageItem* i) { delete i; });
			return node;
		}

		ImageItem(std::string name = "text", bool sameLine = false, int width=64, int height=64)
		{
			_name = name;
			_sameLine = sameLine;
			_img = cv::Mat(height, width, CV_8UC4);
	
			cv::circle(_img, cv::Point2f(32, 32), 15, cv::Scalar(100, 59, 24,255),-1);
            _texture = ToTexture();
			_width = width;
			_height = height;
		}

		void Compute() override
		{

		}

		void PreRender() override
		{
			ImGui::Image((ImTextureID)_texture, ImVec2(_width, _height));	
		}

		void PostRender() override
		{
		}
	private:
		int _width;
		int _height;
		GLuint  _texture;
		cv::Mat _img;

        GLuint ToTexture() {
            GLuint textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            GLenum inputColourFormat = GL_BGRA;

         
            glTexImage2D(GL_TEXTURE_2D,
                0,                
                GL_BGRA,          
				_width,
				_height,
                0,                 
                inputColourFormat, 
                GL_UNSIGNED_BYTE,  
                _img.data);        
            return textureID;
        }
	};
}
