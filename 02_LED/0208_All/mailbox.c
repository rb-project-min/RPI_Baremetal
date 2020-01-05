#include "mailbox.h"

// ���[���{�b�N�X���b�Z�[�W�o�b�t�@
volatile uint32_t __attribute__((aligned(16))) mbox[256];

void gpio_set_config(uint32_t ExpGPIO, bool output, bool active_low, bool term_en, bool pull_up, bool initial_state)
{
	mbox[0] = 12 * 4;								// �o�b�t�@�T�C�Y(�w�b�_�[����p�f�B���O�܂�)
	mbox[1] = RPI_FIRMWARE_STATUS_REQUEST;			// ���N�G�X�g/���X�|���X

	mbox[2] = RPI_FIRMWARE_SET_GPIO_CONFIG;			// �^�O
	mbox[3] = 6 * 4;								// �o�b�t�@�T�C�Y(���̃^�O�̉����̂�)
	mbox[4] = RPI_FIRMWARE_STATUS_REQUEST;			// ���N�G�X�g/���X�|���X
	mbox[5] = ExpGPIO + 128;						// ExpGPIO No.
	mbox[6] = output ? 1 : 0;						// ExpGPIO�̓��o�� 0:���� 1:�o��
	mbox[7] = active_low ? 1 : 0;					// ExpGPIO�̔��] 0:�Ȃ� 1:����
	mbox[8] = term_en ? 1 : 0;						// ExpGPIO�̃v���A�b�v/�_�E���̗L�� 0:�Ȃ� 1:����
	mbox[9] = pull_up ? 1 : 0;						// ExpGPIO�̃v���A�b�v/�_�E�� 0:�v���_�E�� 1:�v���A�b�v
	mbox[10] = initial_state ? 1 : 0;				// ExpGPIO�̏����l 0:LOW 1:HIGH (���]�� 0:HIGH 1:LOW)
	mbox[11] = RPI_FIRMWARE_PROPERTY_END;

	mailbox(mbox, 8);
}

uint32_t gpio_get_config(uint32_t ExpGPIO, bool *output, bool *active_low, bool *term_en, bool *pull_up, bool *state)
{
	mbox[0] = 12 * 4;								// �o�b�t�@�T�C�Y(�w�b�_�[����p�f�B���O�܂�)
	mbox[1] = RPI_FIRMWARE_STATUS_REQUEST;			// ���N�G�X�g/���X�|���X

	mbox[2] = RPI_FIRMWARE_GET_GPIO_CONFIG;			// �^�O
	mbox[3] = 6 * 4;								// �o�b�t�@�T�C�Y(���̃^�O�̉����̂�)
	mbox[4] = RPI_FIRMWARE_STATUS_REQUEST;							// ���N�G�X�g/���X�|���X
	mbox[5] = ExpGPIO + 128;						// ExpGPIO No.
	mbox[6] = 0;									// ExpGPIO�̓��o�� 0:���� 1:�o��
	mbox[7] = 0;									// ExpGPIO�̔��] 0:�Ȃ� 1:����
	mbox[8] = 0;									// ExpGPIO�̃v���A�b�v/�_�E���̗L�� 0:�Ȃ� 1:����
	mbox[9] = 0;									// ExpGPIO�̃v���A�b�v/�_�E�� 0:�v���_�E�� 1:�v���A�b�v
	mbox[10] = 0;									// ExpGPIO�̏����l 0:LOW 1:HIGH (���]�� 0:HIGH 1:LOW)
	mbox[11] = RPI_FIRMWARE_PROPERTY_END;

	mailbox(mbox, 8);

	*output = mbox[6];
	*active_low = mbox[7];
	*term_en = mbox[8];
	*pull_up = mbox[9];
	*state = mbox[10];
	
	return ExpGPIO;
}

void mailbox_SetGpioState(uint32_t ExpGPIO, bool state)
{
	mbox[0] = 8 * 4;								// �o�b�t�@�T�C�Y(�w�b�_�[����p�f�B���O�܂�)
	mbox[1] = RPI_FIRMWARE_STATUS_REQUEST;			// ���N�G�X�g/���X�|���X

	mbox[2] = RPI_FIRMWARE_SET_GPIO_STATE;			// �^�O
	mbox[3] = 8;									// �o�b�t�@�T�C�Y(���̃^�O�̉����̂�)
	mbox[4] = RPI_FIRMWARE_STATUS_REQUEST;							// ���N�G�X�g/���X�|���X
	mbox[5] = ExpGPIO + 128;						// ExpGPIO No.
	mbox[6] = state ? 1 : 0;						// ExpGPIO�̏�� 0:OFF 1:ON
	mbox[7] = RPI_FIRMWARE_PROPERTY_END;

	mailbox(mbox, 8);
}

bool mailbox_GetGpioState(uint32_t ExpGPIO)
{
	mbox[0] = 8 * 4;								// �o�b�t�@�T�C�Y(�w�b�_�[����p�f�B���O�܂�)
	mbox[1] = RPI_FIRMWARE_STATUS_REQUEST;			// ���N�G�X�g/���X�|���X

	mbox[2] = RPI_FIRMWARE_GET_GPIO_STATE;			// �^�O
	mbox[3] = 8;									// �o�b�t�@�T�C�Y(���̃^�O�̉����̂�)
	mbox[4] = RPI_FIRMWARE_STATUS_REQUEST;							// ���N�G�X�g/���X�|���X
	mbox[5] = ExpGPIO + 128;						// ExpGPIO No.
	mbox[6] = 0;									// ExpGPIO�̏�� 0:OFF 1:ON
	mbox[7] = RPI_FIRMWARE_PROPERTY_END;

	mailbox(mbox, 8);

	return mbox[6];
}

extern uint32_t BoardRevision;

uint32_t mailbox_GetBoardRevision()
{
	return BoardRevision;
}
