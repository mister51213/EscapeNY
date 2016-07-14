////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "texture.h"

Texture::Texture()
{
	m_targaData = 0;
	m_texture = 0;
	m_textureView = 0;
}

Texture::Texture(const Texture& other)
{
}

Texture::~Texture()
{
}

// First load the targa data into an array then create a texture and load 
// the targa data into it in the correct format (targa images are upside down 
// by default and need to be reversed). Then once the texture is loaded it will
// create a resource view of the texture for the shader to use for drawing.

bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
// First load the targa file into the m_targaData array. 
// This function will also pass back the height and width of the texture.

	// Load the targa image data into memory.
	result = LoadTarga(filename, height, width);
	if(!result)
	{
		return false;
	}

// Setup description of DirectX texture to load the targa data into. 
// Use the height and width from the targa image data, and set the 
// format to be a 32 bit RGBA texture. Set the SampleDesc to default. 
// Then set the Usage to D3D11_USAGE_DEFAULT which is the better performing memory, 
// Finally, set the MipLevels, BindFlags, and MiscFlags to the settings 
// required for Mipmaped textures. Once the description is complete, call 
// CreateTexture2D to create an empty texture for us. The next step will be 
// to copy the targa data into that empty texture.

	// Setup the description of the texture.
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if(FAILED(hResult))
	{
		return false;
	}

	// Set the row pitch of the targa image data.
	rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if(FAILED(hResult))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_textureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete [] m_targaData;
	m_targaData = 0;

	return true;
}

// The Shutdown function releases the texture data and the three pointers are set to null.
void Texture::Shutdown()
{
	// Release the texture view resource.
	if(m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	// Release the texture.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	// Release the targa data.
	if(m_targaData)
	{
		delete [] m_targaData;
		m_targaData = 0;
	}
	return;
}

// GetTexture is a helper function to provide easy access to 
// the texture view for any shaders that require it for rendering.

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_textureView;
}

// Targa image loading function. Once again note that targa images are 
// stored upside down and need to be flipped before using. 

// *So here we will open the file, read it into an array, and then take 
// that array data and load it into the m_targaData array in the correct order. 
// Note we are purposely only dealing with 32 bit targa files that have alpha channels, 
// this function will reject targa's that are saved as 24 bit.

bool Texture::LoadTarga(char* filename, int& height, int& width)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if(bpp != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];
	if(!targaImage)
	{
		return false;
	}

	// Read in the targa image data.
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}

	// Allocate memory for the targa destination data.
	m_targaData = new unsigned char[imageSize];
	if(!m_targaData)
	{
		return false;
	}

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data.
	k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array 
    // in the correct order since the targa format is stored upside down.
	for(j=0; j<height; j++)
	{
		for(i=0; i<width; i++)
		{
			m_targaData[index + 0] = targaImage[k + 2];  // Red.
			m_targaData[index + 1] = targaImage[k + 1];  // Green.
			m_targaData[index + 2] = targaImage[k + 0];  // Blue
			m_targaData[index + 3] = targaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the 
        // beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}
	// Release the targa image data now that it was copied into the destination array.
	delete [] targaImage;
	targaImage = 0;

	return true;
}