//-----------------------------------------------------------------------------
// 
// 		?c?w?ÅÒ?C?u?ÅÒ??		?}?X?N?????v???O?ÅÒ??( Direct3D11 )
// 
//  	Ver 3.24f
// 
//-----------------------------------------------------------------------------

// ?c?w?ÅÒ?C?u?ÅÒ?????????pÅf??`
#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D11

#ifndef DX_NON_MASK

// ?C?Åg?N???[?h----------------------------------------------------------------
#include <DxLib.h>
#include "../DxStatic.h"
#include "../DxSystem.h"
//#include "../DxBaseFunc.h"
#include "../DxLog.h"
#include "../DxMask.h"
#include "../DxModel.h"
#include "DxMaskD3D11.h"
#include "DxGraphicsD3D11.h"
#include "DxGraphicsWin.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// ?}?N??Åf??`------------------------------------------------------------------

// ?^Åf??`----------------------------------------------------------------------

// ?f?[?^????------------------------------------------------------------------

MASKMANAGEDATA_DIRECT3D11 MaskManageData_D3D11 ;

// ???Åh?v???g?^?C?v???? -------------------------------------------------------

// ?v???O?ÅÒ??------------------------------------------------------------------

// ??????ÅeÅ˜???Åh

// ?}?X?N?X?N???[?Åg?????????????Åh
extern int Mask_D3D11_CreateScreenFunction_Timing0_PF( void )
{
	MASKD3D11.MaskScreenTextureOld    = MASKD3D11.MaskScreenTexture ;
	MASKD3D11.MaskScreenTexture       = NULL ;

	MASKD3D11.MaskScreenTextureOldSRV = MASKD3D11.MaskScreenTextureSRV ;
	MASKD3D11.MaskScreenTextureSRV    = NULL ;

	MASKD3D11.MaskScreenTextureOldRTV = MASKD3D11.MaskScreenTextureRTV ;
	MASKD3D11.MaskScreenTextureRTV    = NULL ;

	if( MASKD3D11.MaskImageTextureSRV != NULL )
	{
		Direct3D11_Release_ShaderResourceView( MASKD3D11.MaskImageTextureSRV ) ;
		MASKD3D11.MaskImageTextureSRV = NULL ;
	}

	if( MASKD3D11.MaskImageTexture != NULL )
	{
		Direct3D11_Release_Texture2D( MASKD3D11.MaskImageTexture ) ;
		MASKD3D11.MaskImageTexture = NULL ;
	}

	// ?????I??
	return 0 ;
}


// ?}?X?N?X?N???[?Åg?????????????Åh
extern int Mask_D3D11_CreateScreenFunction_Timing1_PF( int Width, int Height )
{
	D_D3D11_TEXTURE2D_DESC				Texture2DDesc ;
	D_D3D11_RENDER_TARGET_VIEW_DESC		RTVDesc ;
	D_D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc ;
	HRESULT								hr ;
	D_D3D11_BOX							DestBox ;

	// ?J?ÅÒ?[?o?b?t?@???A???t?@?o?b?t?@???????sÅÒ?Åh\?????Åˆ?????G?ÅÒ?[
	if( GD3D11.Device.Caps.MaskColorFormat == D_DXGI_FORMAT_UNKNOWN )
	{
		return DXST_LOGFILE_ADDUTF16LE( "\xde\x30\xb9\x30\xaf\x30\xcf\x63\x3b\x75\x28\x75\xab\x30\xe9\x30\xfc\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x67\x30\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x00"/*@ L"?}?X?N?`ÅÒ??p?J?ÅÒ?[?o?b?t?@???g?p???????e?N?X?`???t?H?[?}?b?g??????????????????" @*/ ) ;
	}

	if( GD3D11.Device.Caps.MaskAlphaFormat == D_DXGI_FORMAT_UNKNOWN )
	{
		return DXST_LOGFILE_ADDUTF16LE( "\xde\x30\xb9\x30\xaf\x30\xcf\x63\x3b\x75\x28\x75\xa2\x30\xeb\x30\xd5\x30\xa1\x30\xc1\x30\xe3\x30\xf3\x30\xcd\x30\xeb\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x67\x30\x7f\x4f\x28\x75\x67\x30\x4d\x30\x8b\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x00"/*@ L"?}?X?N?`ÅÒ??p?A???t?@?`???Åg?l???o?b?t?@???g?p???????e?N?X?`???t?H?[?}?b?g??????????????????" @*/ ) ;
	}

	// ÅÒ????T?C?Y???Z?b?g
	MASKD3D11.MaskTextureSizeX = Width ;
	MASKD3D11.MaskTextureSizeY = Height ;

	// ?}?X?N?p?C???[?W?e?N?X?`????????
	if( MASKD3D11.MaskImageTexture == NULL )
	{
		_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) ) ;
		Texture2DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
		Texture2DDesc.Format             = GD3D11.Device.Caps.MaskAlphaFormat ;
		Texture2DDesc.BindFlags          = D_D3D11_BIND_SHADER_RESOURCE ;
		Texture2DDesc.Width              = ( UINT )MASKD.MaskBufferSizeX ;
		Texture2DDesc.Height             = ( UINT )MASKD.MaskBufferSizeY ;
		Texture2DDesc.CPUAccessFlags     = 0 ;
		Texture2DDesc.MipLevels          = 1 ;
		Texture2DDesc.ArraySize          = 1 ;
		Texture2DDesc.SampleDesc.Count   = 1 ;
		Texture2DDesc.SampleDesc.Quality = 0 ;
		hr = D3D11Device_CreateTexture2D_ASync( &Texture2DDesc, NULL, &MASKD3D11.MaskImageTexture ) ;
		if( FAILED( hr ) )
		{
			return DXST_LOGFILE_ADDUTF16LE( "\xde\x30\xb9\x30\xaf\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x28\x75\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"?}?X?N?C???[?W?p?e?N?X?`????????????Åhs????????" @*/ ) ;
		}

		_MEMSET( &SRVDesc, 0, sizeof( SRVDesc ) );
		SRVDesc.Format              = GD3D11.Device.Caps.MaskAlphaFormat ;
		SRVDesc.ViewDimension       = D_D3D11_SRV_DIMENSION_TEXTURE2D ;
		SRVDesc.Texture2D.MipLevels = 1 ;
		hr = D3D11Device_CreateShaderResourceView_ASync( MASKD3D11.MaskImageTexture, &SRVDesc, &MASKD3D11.MaskImageTextureSRV ) ;
		if( FAILED( hr ) )
		{
			return DXST_LOGFILE_ADDUTF16LE( "\xde\x30\xb9\x30\xaf\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xea\x30\xbd\x30\xfc\x30\xb9\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"?}?X?N?C???[?W?e?N?X?`???p?V?F?[?_?[???\?[?X?r?Åc?[??????????Åhs????????" @*/ ) ;
		}

		// ?}?X?N?p?C???[?W?e?N?X?`?????ÅÒ??ÅÒ?
		{
			DestBox.left   = 0 ;
			DestBox.top    = 0 ;
			DestBox.front  = 0 ;
			DestBox.right  = ( UINT )MASKD.MaskBufferSizeX ;
			DestBox.bottom = ( UINT )MASKD.MaskBufferSizeY ;
			DestBox.back   = 1 ;

			D3D11DeviceContext_UpdateSubresource_ASync(
				MASKD3D11.MaskImageTexture,
				D_D3D11CalcSubresource( 0, 0, 1 ),
				&DestBox,
				MASKD.MaskBuffer, ( UINT )MASKD.MaskBufferPitch, 0 ) ;
		}
	}

	// ?}?X?N?p?X?N???[?Åg?e?N?X?`????????
	if( MASKD3D11.MaskScreenTexture == NULL )
	{
		_MEMSET( &Texture2DDesc, 0, sizeof( Texture2DDesc ) ) ;
		Texture2DDesc.Usage              = D_D3D11_USAGE_DEFAULT ;
		Texture2DDesc.Format             = GD3D11.Device.Caps.ScreenFormat ;
		Texture2DDesc.BindFlags          = D_D3D11_BIND_SHADER_RESOURCE | D_D3D11_BIND_RENDER_TARGET ;
		Texture2DDesc.Width              = ( UINT )MASKD.MaskBufferSizeX ;
		Texture2DDesc.Height             = ( UINT )MASKD.MaskBufferSizeY ;
		Texture2DDesc.CPUAccessFlags     = 0 ;
		Texture2DDesc.MipLevels          = 1 ;
		Texture2DDesc.ArraySize          = 1 ;
		Texture2DDesc.SampleDesc.Count   = 1 ;
		Texture2DDesc.SampleDesc.Quality = 0 ;
		hr = D3D11Device_CreateTexture2D_ASync( &Texture2DDesc, NULL, &MASKD3D11.MaskScreenTexture ) ;
		if( FAILED( hr ) )
		{
			DXST_LOGFILE_ADDUTF16LE( "\xde\x30\xb9\x30\xaf\x30\x28\x75\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"?}?X?N?p?X?N???[?Åg?e?N?X?`????????????Åhs????????\n" @*/ ) ;
			return -1 ;
		}

		_MEMSET( &SRVDesc, 0, sizeof( SRVDesc ) );
		SRVDesc.Format              = GD3D11.Device.Caps.ScreenFormat ;
		SRVDesc.ViewDimension       = D_D3D11_SRV_DIMENSION_TEXTURE2D ;
		SRVDesc.Texture2D.MipLevels = 1 ;
		hr = D3D11Device_CreateShaderResourceView_ASync( MASKD3D11.MaskScreenTexture, &SRVDesc, &MASKD3D11.MaskScreenTextureSRV ) ;
		if( FAILED( hr ) )
		{
			return DXST_LOGFILE_ADDUTF16LE( "\xde\x30\xb9\x30\xaf\x30\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\xea\x30\xbd\x30\xfc\x30\xb9\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"?}?X?N?X?N???[?Åg?e?N?X?`???p?V?F?[?_?[???\?[?X?r?Åc?[??????????Åhs????????" @*/ ) ;
		}

		_MEMSET( &RTVDesc, 0, sizeof( RTVDesc ) );
		RTVDesc.Format        = GD3D11.Device.Caps.ScreenFormat ;
		RTVDesc.ViewDimension = D_D3D11_RTV_DIMENSION_TEXTURE2D ;
		hr = D3D11Device_CreateRenderTargetView_ASync( MASKD3D11.MaskScreenTexture, &RTVDesc, &MASKD3D11.MaskScreenTextureRTV ) ;
		if( FAILED( hr ) )
		{
			return DXST_LOGFILE_ADDUTF16LE( "\xde\x30\xb9\x30\xaf\x30\xb9\x30\xaf\x30\xea\x30\xfc\x30\xf3\x30\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\x28\x75\xec\x30\xf3\x30\xc0\x30\xfc\x30\xbf\x30\xfc\x30\xb2\x30\xc3\x30\xc8\x30\xd3\x30\xe5\x30\xfc\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"?}?X?N?X?N???[?Åg?e?N?X?`???p???Åg?_?[?^?[?Q?b?g?r?Åc?[??????????Åhs????????" @*/ ) ;
		}
	}

	// ?????I??
	return 0 ;
}

// ?}?X?N?X?N???[?Åg?????????????Åh
extern	int			Mask_D3D11_CreateScreenFunction_Timing2_PF( int MaskBufferSizeXOld, int MaskBufferSizeYOld )
{
	RECT UpdateRect ;
	D_D3D11_BOX DestBox ;
	int i ;

	// ?}?X?N?p?C???[?W?e?N?X?`????Åg??e??Åg]Åe?
	DestBox.left   = 0 ;
	DestBox.top    = 0 ;
	DestBox.front  = 0 ;
	DestBox.right  = ( UINT )MaskBufferSizeXOld ;
	DestBox.bottom = ( UINT )MaskBufferSizeYOld ;
	DestBox.back   = 1 ;

	D3D11DeviceContext_UpdateSubresource_ASync(
		MASKD3D11.MaskImageTexture,
		D_D3D11CalcSubresource( 0, 0, 1 ),
		&DestBox,
		MASKD.MaskBuffer, ( UINT )MASKD.MaskBufferPitch, 0 ) ;

	// ?}?X?N?p?X?N???[?Åg?e?N?X?`????????????Åg??e??Åg]Åe?
	UpdateRect.left   = 0 ;
	UpdateRect.top    = 0 ;
	UpdateRect.right  = MaskBufferSizeXOld ;
	UpdateRect.bottom = MaskBufferSizeYOld ;
	Graphics_D3D11_StretchRect(
		MASKD3D11.MaskScreenTextureOld, MASKD3D11.MaskScreenTextureOldSRV, &UpdateRect,
		MASKD3D11.MaskScreenTexture,    MASKD3D11.MaskScreenTextureRTV,    &UpdateRect, D_D3D11_FILTER_TYPE_POINT ) ;

	// ???Åg?_?[?^?[?Q?b?g?????????????????X????
	for( i = 0 ; i < DX_RENDERTARGET_COUNT ; i ++ )
	{
		if( GD3D11.Device.State.TargetTexture2D[ i ] == MASKD3D11.MaskScreenTextureOld )
		{
			Graphics_D3D11_DeviceState_SetRenderTarget( MASKD3D11.MaskScreenTexture, MASKD3D11.MaskScreenTextureRTV, i ) ;
		}
	}

	Direct3D11_Release_RenderTargetView(   MASKD3D11.MaskScreenTextureOldRTV ) ;
	Direct3D11_Release_ShaderResourceView( MASKD3D11.MaskScreenTextureOldSRV ) ;
	Direct3D11_Release_Texture2D(          MASKD3D11.MaskScreenTextureOld ) ;
	MASKD3D11.MaskScreenTextureOldRTV = NULL ;
	MASKD3D11.MaskScreenTextureOldSRV = NULL ;
	MASKD3D11.MaskScreenTextureOld    = NULL ;

	// ?????I??
	return 0 ;
}

// ?}?X?N?X?N???[?Åg??????????????
extern	int			Mask_D3D11_ReleaseSurface_PF( void )
{
	if( MASKD3D11.MaskScreenTextureRTV != NULL )
	{
		Direct3D11_Release_RenderTargetView( MASKD3D11.MaskScreenTextureRTV ) ;
		MASKD3D11.MaskScreenTextureRTV = NULL ;
	}

	if( MASKD3D11.MaskScreenTextureSRV != NULL )
	{
		Direct3D11_Release_ShaderResourceView( MASKD3D11.MaskScreenTextureSRV ) ;
		MASKD3D11.MaskScreenTextureSRV = NULL ;
	}

	if( MASKD3D11.MaskScreenTexture != NULL )
	{
		Direct3D11_Release_Texture2D( MASKD3D11.MaskScreenTexture ) ;
		MASKD3D11.MaskScreenTexture = NULL ;
	}

	if( MASKD3D11.MaskImageTextureSRV != NULL )
	{
		Direct3D11_Release_ShaderResourceView( MASKD3D11.MaskImageTextureSRV ) ;
		MASKD3D11.MaskImageTextureSRV = NULL ;
	}

	if( MASKD3D11.MaskImageTexture != NULL )
	{
		Direct3D11_Release_Texture2D( MASKD3D11.MaskImageTexture ) ;
		MASKD3D11.MaskImageTexture = NULL ;
	}

	// ?????I??
	return 0 ;
}

// ?}?X?N?g?p???[?h?????X
extern	int			Mask_D3D11_SetUseMaskScreenFlag_PF( void )
{
	IMAGEDATA *Image = NULL ;
	SHADOWMAPDATA *ShadowMap = NULL ;

	// ?`ÅÒ?????ÅÒ?Åe??f?[?^?A?h???X????Åg?????
	if( GSYS.DrawSetting.ShadowMapDrawSetupRequest )
	{
		ShadowMap = Graphics_ShadowMap_GetData( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;
	}
	else
	{
		Image = Graphics_Image_GetData( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;
	}

	// ?`ÅÒ?Åe??@?????????`ÅÒ??ÅN???`ÅÒ?
	DRAWSTOCKINFO

	// ?`ÅÒ????????X????

	// ?}?X?N?T?[?t?F?X??ÅeÅ˜???????????????L???????Åˆ???}?X?N?T?[?t?F?X???`ÅÒ?Åe?????????
	if( MASKD.MaskValidFlag && MASKD3D11.MaskScreenTexture )
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( MASKD3D11.MaskScreenTexture, MASKD3D11.MaskScreenTextureRTV ) ;
	}
	else
	// ?V???h?E?}?b?v???L???????Åˆ???V???h?E?}?b?v???`ÅÒ?Åe?????????
	if( ShadowMap )
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( ShadowMap->PF->D3D11.DepthTexture, ShadowMap->PF->D3D11.DepthTextureRTV, 0 ) ;
	}
	else
	// ?`ÅÒ?ÅÒ?Åh\ÅÒ?Åe????L???????Åˆ???`ÅÒ?ÅÒ?Åh\ÅÒ?Åe????`ÅÒ?Åe?????????
	if( Image )
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture, Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureRTV[ GSYS.DrawSetting.TargetScreenSurface[ 0 ] ] ) ;
	}
	else
	// ?T?u?o?b?N?o?b?t?@???L???????Åˆ???T?u?o?b?N?o?b?t?@???`ÅÒ?Åe?????????
	if( GD3D11.Device.Screen.SubBackBufferTexture2D != NULL )
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( GD3D11.Device.Screen.SubBackBufferTexture2D, GD3D11.Device.Screen.SubBackBufferRTV ) ;
	}
	else
	// ???????O?????Åˆ???o?b?N?o?b?t?@???`ÅÒ?Åe?????????
	{
		Graphics_D3D11_DeviceState_SetRenderTarget( GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2D, GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferRTV ) ;
	}

	// ?g?p?????y?o?b?t?@???Z?b?g?A?b?v
	Graphics_Screen_SetupUseZBuffer() ;

	// ?r?Åc?[?|?[?g??????????
	GD3D11.Device.DrawSetting.CancelSettingEqualCheck = TRUE ;
	Graphics_D3D11_DeviceState_SetViewportEasy(
		( float )GSYS.DrawSetting.DrawArea.left,
		( float )GSYS.DrawSetting.DrawArea.top,
		( float )GSYS.DrawSetting.DrawArea.right,
		( float )GSYS.DrawSetting.DrawArea.bottom
	) ;
	GD3D11.Device.DrawSetting.CancelSettingEqualCheck = FALSE ;

	// ?????I??
	return 0 ;
}

// ?}?X?N???g?p?????`ÅÒ???ÅeO?????????Åh( ?????????T?u?o?b?t?@???g?p?????`ÅÒ??G???A?@Åh\???g?p???????????Åˆ???????????????????????????? )
extern	int			Mask_D3D11_DrawBeginFunction_PF( RECT *Rect )
{
	IMAGEDATA *Image ;

	// ?`ÅÒ?????ÅÒ?Åe??f?[?^?A?h???X????Åg?????
	Image = Graphics_Image_GetData( GSYS.DrawSetting.TargetScreen[ 0 ] ) ;

	// ?}?X?N???g?p????????????????Åg???????????????
	if( MASKD.MaskValidFlag && MASKD3D11.MaskScreenTexture )
	{
		// ???I?o????????Åf?
		
		// ?`ÅÒ?ÅÒ?Åh\ÅÒ?Åe????`ÅÒ????????Åˆ?????????A???????O?????Åˆ???T?u?o?b?N?o?b?t?@???g?p???????????Åˆ???T?u?o?b?N?o?b?t?@???A
		// ???????????????Åˆ???o?b?N?o?b?t?@???o??????????
		if( Image )
		{
			MASKD3D11.DestTargetTexture2D = Image->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture ;
			MASKD3D11.DestTargetSRV       = Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV ;
			MASKD3D11.DestTargetRTV       = Image->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureRTV[ GSYS.DrawSetting.TargetScreenSurface[ 0 ] ] ;
		}
		else
		// ?T?u?o?b?N?o?b?t?@???L???????Åˆ???T?u?o?b?N?o?b?t?@???`ÅÒ?Åe?????????
		if( GD3D11.Device.Screen.SubBackBufferTexture2D != NULL )
		{
			MASKD3D11.DestTargetTexture2D = GD3D11.Device.Screen.SubBackBufferTexture2D ;
			MASKD3D11.DestTargetSRV       = GD3D11.Device.Screen.SubBackBufferSRV ;
			MASKD3D11.DestTargetRTV       = GD3D11.Device.Screen.SubBackBufferRTV ;
		}
		else
		// ???????O?????Åˆ???o?b?N?o?b?t?@???`ÅÒ?Åe?????????
		{
			MASKD3D11.DestTargetTexture2D = GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferTexture2D ;
			MASKD3D11.DestTargetSRV       = GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferSRV ;
			MASKD3D11.DestTargetRTV       = GD3D11.Device.Screen.OutputWindowInfo[ 0 ].BufferRTV ;
		}

		// ???I?o?????????}?X?N?X?N???[?Åg?????????`ÅÒ????????R?s?[????
		Graphics_D3D11_StretchRect(
			MASKD3D11.DestTargetTexture2D, MASKD3D11.DestTargetSRV,        Rect,
			MASKD3D11.MaskScreenTexture,   MASKD3D11.MaskScreenTextureRTV, Rect, D_D3D11_FILTER_TYPE_POINT ) ; 
	}

	// ?????I??
	return 0 ;
}

// ?}?X?N???g?p?????`ÅÒ??????????????Åh( ?????????T?u?o?b?t?@???g?p?????`ÅÒ??G???A?@Åh\???g?p???????????Åˆ???????????????????????????? )
extern	int			Mask_D3D11_DrawAfterFunction_PF( RECT *Rect )
{
	// ?}?X?N???g?p???????????Åˆ?????}?X?NÅÒ?Åe????Åˆ??????Åg]Åe?
	if( MASKD.MaskValidFlag && MASKD3D11.MaskScreenTexture )
	{
		IMAGEDATA *MaskScreenImage = NULL ;
		D_ID3D11Texture2D          *MaskImageTexture ;
		D_ID3D11ShaderResourceView *MaskImageTextureSRV ;
		D_ID3D11PixelShader        *MaskPixelShader ;
		int                         AlphaBlend ;

		// ?}?X?N?p?O?ÅÒ?t?B?b?N?n?Åg?h?????L???????Åˆ???}?X?N?p?O?ÅÒ?t?B?b?N?n?Åg?h????????????Åg?????
		if( MASKD.MaskScreenGraphHandle != 0 )
		{
			MaskScreenImage = Graphics_Image_GetData( MASKD.MaskScreenGraphHandle ) ;

			// ????????????????????????Åf??? 0 ??????
			if( MaskScreenImage == NULL )
			{
				MASKD.MaskScreenGraphHandle = 0 ;
			}
		}

		// ?g?p?????}?X?N?C???[?W?e?N?X?`?????Z?b?g
		if( MaskScreenImage != NULL )
		{
			MaskImageTexture    = MaskScreenImage->Hard.Draw[ 0 ].Tex->PF->D3D11.Texture ;
			MaskImageTextureSRV = MaskScreenImage->Hard.Draw[ 0 ].Tex->PF->D3D11.TextureSRV ;
			MaskPixelShader     = MASKD.MaskReverseEffectFlag ?
				GD3D11.Device.Shader.Base.MaskEffect_UseGraphHandle_ReverseEffect_PS[ MASKD.MaskScreenGraphHandleUseChannel ] :
				GD3D11.Device.Shader.Base.MaskEffect_UseGraphHandle_PS[ MASKD.MaskScreenGraphHandleUseChannel ] ;
			AlphaBlend          = TRUE ;
		}
		else
		{
			MaskImageTexture    = MASKD3D11.MaskImageTexture ;
			MaskImageTextureSRV = MASKD3D11.MaskImageTextureSRV ;
			MaskPixelShader     = MASKD.MaskReverseEffectFlag ? GD3D11.Device.Shader.Base.MaskEffect_ReverseEffect_PS : GD3D11.Device.Shader.Base.MaskEffect_PS ;
			AlphaBlend          = FALSE ;
		}

		Graphics_D3D11_StretchRect(
			MASKD3D11.MaskScreenTexture,   MASKD3D11.MaskScreenTextureSRV, Rect,
			MASKD3D11.DestTargetTexture2D, MASKD3D11.DestTargetRTV,        Rect,
			D_D3D11_FILTER_TYPE_POINT,
			AlphaBlend,
			NULL, MaskPixelShader,
			MaskImageTexture,              MaskImageTextureSRV,  Rect
		) ;
	}

	// ?????I??
	return 0 ;
}

// ?}?X?N?X?N???[?Åg???wÅf????F??Ågh????????
extern	int			Mask_D3D11_FillMaskScreen_PF( int /*Flag*/ )
{
	D_D3D11_BOX		DestBox ;

	DestBox.left   = 0 ;
	DestBox.top    = 0 ;
	DestBox.front  = 0 ;
	DestBox.right  = ( UINT )MASKD.MaskBufferSizeX ;
	DestBox.bottom = ( UINT )MASKD.MaskBufferSizeY ;
	DestBox.back   = 1 ;

	D3D11DeviceContext_UpdateSubresource_ASync(
		MASKD3D11.MaskImageTexture,
		D_D3D11CalcSubresource( 0, 0, 1 ),
		&DestBox,
		MASKD.MaskBuffer, ( UINT )MASKD.MaskBufferPitch, 0 ) ;

	// ?????I??
	return 0 ;
}

// ?wÅf????????}?X?N?C???[?W?e?N?X?`?????X?V????
extern	int			Mask_D3D11_UpdateMaskImageTexture_PF( RECT *Rect )
{
	D_D3D11_BOX		DestBox ;

	DestBox.left   = ( UINT )Rect->left ;
	DestBox.top    = ( UINT )Rect->top ;
	DestBox.front  = 0 ;
	DestBox.right  = ( UINT )Rect->right ;
	DestBox.bottom = ( UINT )Rect->bottom ;
	DestBox.back   = 1 ;

	D3D11DeviceContext_UpdateSubresource_ASync(
		MASKD3D11.MaskImageTexture,
		D_D3D11CalcSubresource( 0, 0, 1 ),
		&DestBox,
		( BYTE * )MASKD.MaskBuffer + Rect->left + MASKD.MaskBufferPitch * Rect->top, ( UINT )MASKD.MaskBufferPitch, 0 ) ;

	// ?I??
	return 0 ;
}




#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MASK

#endif // DX_NON_DIRECT3D11

#endif // DX_NON_GRAPHICS
