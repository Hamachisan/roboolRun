#pragma once

//------------------���ʂŎg�p����萔���L�q����w�b�_�[�t�@�C��--------------------//

//�X�N���[���T�C�Y
//�������G���main.cpp�ɂ��e���A���B��ʃT�C�Y���ς��
#define SCREEN_WIDTH			(1280)					//�X�N���[������
#define SCREEN_HEIGHT			(720)					//�X�N���[���c��			

//�ő�X�e�[�W��
#define MAX_STAGE (8)
//�X�e�[�W�̃��x����
#define MAX_STAGE_LEVEL (2)

//�Q�[�����X�^�[�g����ʒu...Z��
#define START_LINE				(-390)
//�S�[������ʒu...Z��
#define GOAL_LINE				(1150)

//���̃V�[��
#define NEXT_SCENE_GAME			(1)						//�Q�[���V�[��
#define NEXT_SCENE_OTHER		(0)						//����ȊO�̃V�[��

//��Q���֘A
#define ENEMY_LINE				(30)					//�c�̏�Q���̐�
#define ENEMY_COLUMN			(3)						//���[����
//...�z�u�֘A
#define OBJ_START_LINE			(START_LINE + 55)		//�X�^�[�g�n�_
#define STANDARD_POTITION_X		(-10)					//X���I�u�W�F�N�g�̊�ʒu -10, 0, 10
#define INTERVAL_X				(10)					//X���I�u�W�F�N�g�̊Ԋu
#define INTERVAL_Z				(50)					//Z���I�u�W�F�N�g�̊Ԋu

//�w�i�I�u�W�F�N�g�z�u�֘A
#define BACK_OBJ_MAX			(3)					//�w�i�̊�ő吶����
#define BACK_OBJ_INTERVAL_Z		(100)				//Z���I�u�W�F�N�g�̊Ԋu
#define LEFT_FRONT_LINE			(0)					//����̈�ԑO
#define RIGHT_FRONT_LINE		(3)					//�E��̈�ԑO


//Sprite�`��
#define STANDARD_POS			(500)					//�
#define ABOUT_HALF				(0.48)					//�񔼕�
#define PIXEL_32				(32)					//32�s�N�Z��
#define PIXEL_64				(64)					//64�s�N�Z��
#define PIXEL_128				(128)					//128�s�N�Z��
#define PIXEL_256				(256)					//256�s�N�Z��
#define PIXEL_512				(512)					//512�s�N�Z��