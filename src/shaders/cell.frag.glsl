#version 330 core

out vec4 FragColor;
in vec2 TextCoord;
in float Status;

uniform sampler2D g1Texture;
uniform sampler2D sTexture;
uniform sampler2D g2Texture;
uniform sampler2D proTexture;
uniform sampler2D metaTexture;
uniform sampler2D anaTexture;
uniform sampler2D teloTexture;

void main() {
	if (Status == float(0)) {
		FragColor = texture(g1Texture, TextCoord);
	}
	else if (Status == float(1)) {
		FragColor = texture(sTexture, TextCoord);
	}
	else if (Status == float(2)) {
		FragColor = texture(g2Texture, TextCoord);
	}
	else if (Status == float(3)) {
		FragColor = texture(proTexture, TextCoord);
	}
	else if (Status == float(4)) {
		FragColor = texture(metaTexture, TextCoord);
	}
	else if (Status == float(5)) {
		FragColor = texture(anaTexture, TextCoord);
	}
	else if (Status == float(6)) {
		FragColor = texture(teloTexture, TextCoord);
	}
}
