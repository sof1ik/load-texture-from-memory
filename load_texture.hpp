#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "imgui/stb_image.h"

class CLoadTexture {
public:
	ID3D11Device* g_p3dDevice;

	bool load_texture_from_memory(unsigned char* bytes, size_t size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height) {
		int width = 0;
		int height = 0;

		unsigned char* buf_data = stbi_load_from_memory(bytes, size, &width, &height, NULL, 4);
		if (buf_data == NULL)
			return false;

		// texture;
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D* textured = NULL;
		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = buf_data;
		sub_resource.SysMemPitch = desc.Width * 4;
		sub_resource.SysMemSlicePitch = 0;
		g_p3dDevice->CreateTexture2D(&desc, &sub_resource, &textured);
		// - - -

		// texture view;
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		ZeroMemory(&srv_desc, sizeof(srv_desc));
		srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = desc.MipLevels;
		srv_desc.Texture2D.MostDetailedMip = 0;
		g_p3dDevice->CreateShaderResourceView(textured, &srv_desc, out_srv);
		textured->Release();

		*out_width = width;
		*out_height = height;
		stbi_image_free(buf_data);
		// - - - 

		return true;
	}
};
