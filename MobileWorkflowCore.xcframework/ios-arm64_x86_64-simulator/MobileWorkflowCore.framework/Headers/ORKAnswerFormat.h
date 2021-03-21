/*
 Copyright (c) 2015, Apple Inc. All rights reserved.
 Copyright (c) 2015, Bruce Duncan.
 Copyright (c) 2016, Ricardo Sánchez-Sáez.
 Copyright (c) 2017, Macro Yau.
 Copyright (c) 2017, Sage Bionetworks.
 
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
 
 1.  Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 2.  Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.
 
 3.  Neither the name of the copyright holder(s) nor the names of any contributors
 may be used to endorse or promote products derived from this software without
 specific prior written permission. No license is granted to the trademarks of
 the copyright holders even if such marks are included in this software.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


@import UIKit;
#import <MobileWorkflowCore/ORKTypes.h>


NS_ASSUME_NONNULL_BEGIN

@class ORKValuePickerAnswerFormat;
@class ORKMultipleValuePickerAnswerFormat;
@class ORKTextChoiceAnswerFormat;
@class ORKBooleanAnswerFormat;
@class ORKNumericAnswerFormat;
@class ORKTimeOfDayAnswerFormat;
@class ORKDateAnswerFormat;
@class ORKTextAnswerFormat;
@class ORKEmailAnswerFormat;
@class ORKLocationAnswerFormat;

@class ORKTextChoice;

@class CurrencyFormatter;

/**
 The `ORKAnswerFormat` class is the abstract base class for classes that describe the
 format in which a survey question or form item should be answered. The ResearchKit framework uses
 `ORKQuestionStep` and `ORKFormItem` to represent questions to ask the user. Each
 question must have an associated answer format.
 
 To use an answer format, instantiate the appropriate answer format subclass and
 attach it to a question step or form item. Incorporate the resulting step
 into a task, and present the task with a task view controller.
 
 An answer format is validated when its owning step is validated.
 
 Some answer formats are constructed of other answer formats. When this is the
 case, the answer format can override the method `impliedAnswerFormat` to return
 the answer format that is implied. For example, a Boolean answer format
 is presented in the same way as a single-choice answer format with the
 choices Yes and No mapping to `@(YES)` and `@(NO)`, respectively.
 */
ORK_CLASS_AVAILABLE
@interface ORKAnswerFormat : NSObject <NSSecureCoding, NSCopying>

/// @name Properties

/**
 The type of question. (read-only)
 
 You can use this enumerated value in your Objective-C code to switch on
 a rough approximation of the type of question that is being asked.
 
 Note that answer format subclasses override the getter to return the appropriate question
 type.
 */
@property (readonly) ORKQuestionType questionType;

/**
 Determines if the "I Don't Know" button will show.
 
 Defults to false.
 */
@property (nonatomic, assign, getter=shouldShowDontKnowButton) BOOL showDontKnowButton;

/**
 Custom text that will be shown inside of the "I Don't Know" button if showDontKnowButton is set to true.
 
 Defults to nil.
 */
@property (nonatomic, nullable) NSString *customDontKnowButtonText;

/// @name Factory methods

+ (ORKBooleanAnswerFormat *)booleanAnswerFormat;

+ (ORKBooleanAnswerFormat *)booleanAnswerFormatWithYesString:(NSString *)yes
                                                    noString:(NSString *)no;

+ (ORKValuePickerAnswerFormat *)valuePickerAnswerFormatWithTextChoices:(NSArray<ORKTextChoice *> *)textChoices;

+ (ORKMultipleValuePickerAnswerFormat *)multipleValuePickerAnswerFormatWithValuePickers:(NSArray<ORKValuePickerAnswerFormat *> *)valuePickers;

+ (ORKTextChoiceAnswerFormat *)choiceAnswerFormatWithStyle:(ORKChoiceAnswerStyle)style
                                               textChoices:(NSArray<ORKTextChoice *> *)textChoices;

+ (ORKNumericAnswerFormat *)decimalAnswerFormatWithUnit:(nullable NSString *)unit;
+ (ORKNumericAnswerFormat *)integerAnswerFormatWithUnit:(nullable NSString *)unit;

+ (ORKTimeOfDayAnswerFormat *)timeOfDayAnswerFormat;
+ (ORKTimeOfDayAnswerFormat *)timeOfDayAnswerFormatWithDefaultComponents:(nullable NSDateComponents *)defaultComponents;

+ (ORKDateAnswerFormat *)dateTimeAnswerFormat;
+ (ORKDateAnswerFormat *)dateTimeAnswerFormatWithDefaultDate:(nullable NSDate *)defaultDate
                                                 minimumDate:(nullable NSDate *)minimumDate
                                                 maximumDate:(nullable NSDate *)maximumDate
                                                    calendar:(nullable NSCalendar *)calendar;

+ (ORKDateAnswerFormat *)dateAnswerFormat;
+ (ORKDateAnswerFormat *)dateAnswerFormatWithDefaultDate:(nullable NSDate *)defaultDate
                                             minimumDate:(nullable NSDate *)minimumDate
                                             maximumDate:(nullable NSDate *)maximumDate
                                                calendar:(nullable NSCalendar *)calendar;

+ (ORKTextAnswerFormat *)textAnswerFormat;

+ (ORKTextAnswerFormat *)textAnswerFormatWithMaximumLength:(NSInteger)maximumLength;

+ (ORKTextAnswerFormat *)textAnswerFormatWithValidationRegularExpression:(NSRegularExpression *)validationRegularExpression
                                                          invalidMessage:(NSString *)invalidMessage;

+ (ORKEmailAnswerFormat *)emailAnswerFormat;

+ (ORKLocationAnswerFormat *)locationAnswerFormat;

/// @name Validation

/**
 Validates the parameters of the answer format to ensure that they can be displayed.
 
 Typically, this method is called by the validation methods of the owning objects, which are
 themselves called when a step view controller that contains this answer format is
 about to be displayed.
 */
- (void)validateParameters;

/**
 Some answer formats are constructed of other answer formats. This method allows
 a subclass to return a different answer format for use in defining the UI/UX for
 the answer format type. For example, a Boolean answer format is presented in the 
 same way as a single-choice answer format with the choices Yes and No mapping to 
 `@(YES)` and `@(NO)`, respectively, so its `impliedAnswerFormat` is an 
 `ORKTextChoiceAnswerFormat` with those options.
 */
- (ORKAnswerFormat *)impliedAnswerFormat;

@end


/**
 The `ORKValuePickerAnswerFormat` class represents an answer format that lets participants use a
 value picker to choose from a fixed set of text choices.
 
 When the number of choices is relatively large and the text that describes each choice
 is short, you might want to use the value picker answer format instead of the text choice answer
 format (`ORKTextChoiceAnswerFormat`). When the text that describes each choice is long, or there
 are only a very small number of choices, it's usually better to use the text choice answer format.
 
 Note that the value picker answer format reports itself as being of the single choice question
 type. The value picker answer format produces an `ORKChoiceQuestionResult` object.
 */
ORK_CLASS_AVAILABLE
@interface ORKValuePickerAnswerFormat : ORKAnswerFormat

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/**
 Returns a value picker answer format using the specified array of text choices.
 
 Note that the `detailText` property of each choice is ignored. Be sure to create localized text for
 each choice that is short enough to fit in a `UIPickerView` object.
 
 @param textChoices     Array of `ORKTextChoice` objects.
 
 @return An initialized value picker answer format.
 */
- (instancetype)initWithTextChoices:(NSArray<ORKTextChoice *> *)textChoices NS_DESIGNATED_INITIALIZER;

/**
 An array of text choices that represent the options to display in the picker. (read-only)
 
 Note that the `detailText` property of each choice is ignored. Be sure to create localized text for
 each choice that is short enough to fit in a `UIPickerView` object.
 */
@property (copy, readonly) NSArray<ORKTextChoice *> *textChoices;

@end


/**
 The `ORKMultipleValuePickerAnswerFormat` class represents an answer format that lets participants use a
 multiple-component value picker to choose from a fixed set of text choices.
 
 Note that the multiple value picker answer format reports itself as being of the multiple picker question
 type. The multiple-component value picker answer format produces an `ORKMultipleComponentQuestionResult` 
 object where the index into the array matches the array of `ORKValuePickerAnswerFormat` objects.
 
 For example, if the picker shows two columns with choices of `[[A, B, C], [1, 2, 3, 4]]` and the user picked
 `B` and `3` then this would result in `componentsAnswer = [B, 3]`.
 */
ORK_CLASS_AVAILABLE
@interface ORKMultipleValuePickerAnswerFormat : ORKAnswerFormat

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/**
 Returns a multiple value picker answer format using the specified array of value pickers.
 
 @param valuePickers     Array of `ORKValuePickerAnswerFormat` objects.
 
 @return An initialized multiple value picker answer format.
 */
- (instancetype)initWithValuePickers:(NSArray<ORKValuePickerAnswerFormat *> *)valuePickers;

/**
 Returns a multiple value picker answer format using the specified array of value pickers.
 
 @param valuePickers     Array of `ORKValuePickerAnswerFormat` objects.
 @param separator        String used to separate the components
 
 @return An initialized multiple value picker answer format.
 */
- (instancetype)initWithValuePickers:(NSArray<ORKValuePickerAnswerFormat *> *)valuePickers separator:(NSString *)separator NS_DESIGNATED_INITIALIZER;

/**
 An array of value pickers that represent the options to display in the picker. (read-only)
 */
@property (copy, readonly) NSArray<ORKValuePickerAnswerFormat *> *valuePickers;

/**
 A string used to define the separator for the format of the string. Default = " ".
 */
@property (copy, readonly) NSString *separator;

@end


/**
 The `ORKTextChoiceAnswerFormat` class represents an answer format that lets participants choose
 from a fixed set of text choices in a multiple or single choice question.
 
 The text choices are presented in a table view, using one row for each answer.
 The text for each answer is given more prominence than the `detailText` in the row, but
 both are shown.
 
 The text choice answer format produces an `ORKChoiceQuestionResult` object.
 */
ORK_CLASS_AVAILABLE
@interface ORKTextChoiceAnswerFormat : ORKAnswerFormat

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/**
 Returns an initialized text choice answer format using the specified question style and array of
 text choices.
 
 @param style           The style of question, such as single or multiple choice.
 @param textChoices     An array of `ORKTextChoice` objects.
 
 @return An initialized text choice answer format.
 */
- (instancetype)initWithStyle:(ORKChoiceAnswerStyle)style
                  textChoices:(NSArray<ORKTextChoice *> *)textChoices NS_DESIGNATED_INITIALIZER;

/**
 The style of the question (that is, single or multiple choice).
 */
@property (readonly) ORKChoiceAnswerStyle style;

/**
 An array of `ORKTextChoice` objects that represent the choices that are displayed to participants.
 
 The choices are presented as a table view, using one row for each answer.
 The text for each answer is given more prominence than the `detailText` in the row, but
 both are shown.
 */
@property (copy, readonly) NSArray<ORKTextChoice *> *textChoices;

@end


/**
 The `ORKBooleanAnswerFormat` class behaves the same as the `ORKTextChoiceAnswerFormat` class,
 except that it is preconfigured to use only Yes and No answers.
 
 The Boolean answer format produces an `ORKBooleanQuestionResult` object.
 */
ORK_CLASS_AVAILABLE
@interface ORKBooleanAnswerFormat : ORKAnswerFormat

/**
 Returns an initialized Boolean answer format using the specified strings for Yes and No answers.
 
 @param yes         A string that describes the Yes answer.
 @param no          A string that describes the No answer.
 
 @return An initialized Boolean answer format.
 */
- (instancetype)initWithYesString:(NSString *)yes noString:(NSString *)no;

/**
 The string to describe the Yes answer. (read-only)
 */
@property (copy, readonly) NSString *yes;

/**
 The string to describe the No answer. (read-only)
 */
@property (copy, readonly) NSString *no;

@end


/**
 The `ORKTextChoice` class defines the text for a choice in answer formats such
 as `ORKTextChoiceAnswerFormat` and `ORKValuePickerAnswerFormat`.
 
 When a participant chooses a text choice item, the value recorded in a result
 is specified by the `value` property.
 */
ORK_CLASS_AVAILABLE
@interface ORKTextChoice : NSObject <NSSecureCoding, NSCopying, NSObject>

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/**
 Returns a text choice object that includes the specified primary text or text with string attributes, detail text or text with string attributes, and exclusivity.
 
 @param text                         The primary text that describes the choice in a localized string.
 @param primaryTextAttributedString  The primary text that describes the choice in an attributed string. Setting this will override `text`.
 @param detailText                   The detail text to display below the primary text, in a localized string.
 @param detailTextAttributedString   The detail text to display below the primary text, in an attributed string. Setting this will override `detailText`.
 @param value                        The value to record in a result object when this item is selected.
 @param exclusive                    Whether this choice is to be considered exclusive within the set of choices.
 
 @return A text choice instance.
 */
+ (instancetype)choiceWithText:(nullable NSString *)text primaryTextAttributedString:(nullable NSAttributedString *)primaryTextAttributedString detailText:(nullable NSString *)detailText detailTextAttributedString:(nullable NSAttributedString *)detailTextAttributedString value:(id<NSCopying, NSCoding, NSObject>)value exclusive:(BOOL)exclusive;

/**
 Returns a text choice object that includes the specified primary text, detail text,
 and exclusivity.
 
 @param text        The primary text that describes the choice in a localized string.
 @param detailText  The detail text to display below the primary text, in a localized string.
 @param value       The value to record in a result object when this item is selected.
 @param exclusive   Whether this choice is to be considered exclusive within the set of choices.
 
 @return A text choice instance.
 */
+ (instancetype)choiceWithText:(NSString *)text detailText:(nullable NSString *)detailText value:(id<NSCopying, NSCoding, NSObject>)value exclusive:(BOOL)exclusive;

/**
 Returns a choice object that includes the specified primary text.
 
 @param text        The primary text that describes the choice in a localized string.
 @param value       The value to record in a result object when this item is selected.
 
 @return A text choice instance.
 */
+ (instancetype)choiceWithText:(NSString *)text value:(id<NSCopying, NSCoding, NSObject>)value;

/**
 Returns an initialized text choice object using the specified primary text, detail text,
 and exclusivity.
 
 @param text        The primary text that describes the choice in a localized string.
 @param detailText  The detail text to display below the primary text, in a localized string.
 @param value       The value to record in a result object when this item is selected.
 @param exclusive   Whether this choice is to be considered exclusive within the set of choices.
 
 @return An initialized text choice.
 */
- (instancetype)initWithText:(NSString *)text
                  detailText:(nullable NSString *)detailText
                       value:(id<NSCopying, NSCoding, NSObject>)value
                    exclusive:(BOOL)exclusive;

/**
 Returns an initialized text choice object using the specified primary text or text with string attributes, detail text or text with string attributes, and exclusivity.
 
 This method is the designated initializer.
 
 @param text                         The primary text that describes the choice in a localized string.
 @param primaryTextAttributedString  The primary text that describes the choice in an attributed string. Setting this will override `text`.
 @param detailText                   The detail text to display below the primary text, in a localized string.
 @param detailTextAttributedString   The detail text to display below the primary text, in an attributed string. Setting this will override `detailText`.
 @param value                        The value to record in a result object when this item is selected.
 @param exclusive                    Whether this choice is to be considered exclusive within the set of choices.
 
 @return An initialized text choice.
 */
- (instancetype)initWithText:(nullable NSString *)text
 primaryTextAttributedString:(nullable NSAttributedString *)primaryTextAttributedString
                  detailText:(nullable NSString *)detailText
  detailTextAttributedString:(nullable NSAttributedString *)detailTextAttributedString
                       value:(id<NSCopying, NSCoding, NSObject>)value
                   exclusive:(BOOL)exclusive NS_DESIGNATED_INITIALIZER;

/**
 The text that describes the choice in a localized string.
 
 In general, it's best when the text can fit on one line.
  */
@property (copy, readonly) NSString *text;

/**
 The text that describes the choice in an attributed string.
 
 In general, it's best when the text can fit on one line.
 */
@property (copy, readonly, nullable) NSAttributedString *primaryTextAttributedString;

/**
 The value to return when this choice is selected.
 
 The value of this property is expected to be a scalar property list type, such as `NSNumber`
 or `NSString`. If no value is provided, the index of the option in the options list in the
 answer format is used.
 */
@property (copy, readonly) id<NSCopying, NSCoding, NSObject> value;

/**
 The text that provides additional details about the choice in a localized string.
 
 The detail text can span multiple lines. Note that `ORKValuePickerAnswerFormat` ignores detail
 text.
  */
@property (copy, readonly, nullable) NSString *detailText;

/**
 The text that provides additional details about the choice in an attributed string.
 
 The detail text can span multiple lines. Note that `ORKValuePickerAnswerFormat` ignores detail
 text.
 */
@property (copy, readonly, nullable) NSAttributedString *detailTextAttributedString;

/**
 In a multiple choice format, this indicates whether this choice requires all other choices to be
 unselected.
 
 In general, this is used to indicate a "None of the above" choice.
 */
@property (readonly) BOOL exclusive;

@end


/**
 The `ORKTextChoiceOther` class defines the choice option to describe an answer not
 included in provided choices.
 
 The `ORKTextChoiceOther` provides an optional text view of type `ORKAnswerTextView` that allows users to enter free form text.
 */
ORK_CLASS_AVAILABLE
@interface ORKTextChoiceOther : ORKTextChoice

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/**
 Returns a text choice other object that includes the specified text, detail text, exclusivity and a text view with placeholder text for additional user input.
 
 @param text                         The primary text that describes the choice in a localized string.
 @param detailText                   The detail text to display below the primary text, in a localized string.
 @param value                        The value to record in a result object when this item is selected.
 @param exclusive                    Whether this choice is to be considered exclusive within the set of choices.
 @param textViewPlaceholderText      The placeholder text for the text view.
 
 @return A text choice other instance.
 */
+ (instancetype)choiceWithText:(nullable NSString *)text
                    detailText:(nullable NSString *)detailText
                         value:(id<NSCopying, NSCoding, NSObject>)value
                     exclusive:(BOOL)exclusive
       textViewPlaceholderText:(NSString *)textViewPlaceholderText;

/**
 Returns an initialized text choice other object using the specified primary text or text with string attributes, detail text or text with string attributes, exclusivity and an optional view for free form text entry.
 
 This method is the designated initializer.
 
 @param text                         The primary text that describes the choice in a localized string.
 @param primaryTextAttributedString  The primary text that describes the choice in an attributed string. Setting this will override `text`.
 @param detailText                   The detail text to display below the primary text, in a localized string.
 @param detailTextAttributedString   The detail text to display below the primary text, in an attributed string. Setting this will override `detailText`.
 @param value                        The value to record in a result object when this item is selected.
 @param exclusive                    Whether this choice is to be considered exclusive within the set of choices.
 @param textViewPlaceholderText      The placeholder text for the text view.
 @param textViewInputOptional        Whether the user is required to provide additional text when selecting this choice.
 @param textViewStartsHidden         Whether the text view should be hidden untill the cell is selected.
 
 @return An initialized text choice other object.
 */
- (instancetype)initWithText:(nullable NSString *)text
 primaryTextAttributedString:(nullable NSAttributedString *)primaryTextAttributedString
                  detailText:(nullable NSString *)detailText
  detailTextAttributedString:(nullable NSAttributedString *)detailTextAttributedString
                       value:(id<NSCopying, NSCoding, NSObject>)value
                   exclusive:(BOOL)exclusive
     textViewPlaceholderText:(NSString *)textViewPlaceholderText
       textViewInputOptional:(BOOL)textViewInputOptional
        textViewStartsHidden:(BOOL)textViewStartsHidden;

@property (copy, readonly, nullable) NSString *textViewPlaceholderText;

@property (readonly) BOOL textViewInputOptional;

@property (readonly) BOOL textViewStartsHidden;

@end


/**
 The `ORKImageChoice` class defines a choice that can
 be included in an `ORKImageChoiceAnswerFormat` object.
 
 Typically, image choices are displayed in a horizontal row, so you need to use appropriate sizes.
 For example, when five image choices are displayed in an `ORKImageChoiceAnswerFormat`, image sizes
 of about 45 to 60 points allow the images to look good in apps that run on all versions of iPhone.
 
 The text that describes an image choice should be reasonably short. However, only the text for the
 currently selected image choice is displayed, so text that wraps to more than one line
 is supported.
 */
ORK_CLASS_AVAILABLE
@interface ORKImageChoice : NSObject <NSSecureCoding, NSCopying>

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/**
 Returns an image choice that includes the specified images and text.
 
 @param normal      The image to display in the unselected state.
 @param selected    The image to display in the selected state.
 @param text        The text to display when the image is selected.
 @param value       The value to record in a result object when the image is selected.
 
 @return An image choice instance.
 */
+ (instancetype)choiceWithNormalImage:(nullable UIImage *)normal
                        selectedImage:(nullable UIImage *)selected
                                 text:(nullable NSString *)text
                                value:(id<NSCopying, NSCoding, NSObject>)value;

/**
 Returns an initialized image choice using the specified images and text.
 
 This method is the designated initializer.
 
 @param normal      The image to display in the unselected state.
 @param selected    The image to display in the selected state.
 @param text        The text to display when the image is selected.
 @param value       The value to record in a result object when the image is selected.
 
 @return An initialized image choice.
 */
- (instancetype)initWithNormalImage:(nullable UIImage *)normal
                      selectedImage:(nullable UIImage *)selected
                               text:(nullable NSString *)text
                              value:(id<NSCopying, NSCoding, NSObject>)value NS_DESIGNATED_INITIALIZER;

/**
 The image to display when the choice is not selected. (read-only)
 
 The size of the unselected image depends on the number of choices you need to display. As a
 general rule, it's recommended that you start by creating an image that measures 44 x 44 points,
 and adjust it if necessary.
 */
@property (strong, readonly) UIImage *normalStateImage;

/**
 The image to display when the choice is selected. (read-only)
 
 For best results, the selected image should be the same size as the unselected image (that is,
 the value of the `normalStateImage` property).
 If you don't specify a selected image, the default `UIButton` behavior is used to
 indicate the selection state of the item.
 */
@property (strong, readonly, nullable) UIImage *selectedStateImage;

/**
 The text to display when the image is selected, in a localized string. (read-only)
 
 Note that the text you supply may be spoken by VoiceOver even when the item is not selected.
  */
@property (copy, readonly, nullable) NSString *text;

/**
 The value to return when the image is selected. (read-only)
 
 The value of this property is expected to be a scalar property list type, such as `NSNumber` or
 `NSString`. If no value is provided, the index of the option in the `ORKImageChoiceAnswerFormat`
 options list is used.
 */
@property (copy, readonly) id<NSCopying, NSCoding, NSObject> value;

@end


/**
 The style of answer for an `ORKNumericAnswerFormat` object, which controls the keyboard that is
 presented during numeric entry.
 */
typedef NS_ENUM(NSInteger, ORKNumericAnswerStyle) {

    /**
     A decimal question type asks the participant to enter a decimal number.
     */
    ORKNumericAnswerStyleDecimal,
    
    /**
     An integer question type asks the participant to enter an integer number.
     */
    ORKNumericAnswerStyleInteger
} ORK_ENUM_AVAILABLE;

/**
 The `ORKNumericAnswerFormat` class defines the attributes for a numeric
 answer format that participants enter using a numeric keyboard.
 
 If you specify maximum or minimum values and the user enters a value outside the
 specified range, the question step view controller does not allow navigation
 until the participant provides a value that is within the valid range.
 
 Questions and form items that use this answer format produce an
 `ORKNumericQuestionResult` object.
 */
ORK_CLASS_AVAILABLE
@interface ORKNumericAnswerFormat : ORKAnswerFormat

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/**
 Returns an initialized numeric answer format using the specified style.
 
 @param style       The style of the numeric answer (decimal or integer).
 
 @return An initialized numeric answer format.
 */
- (instancetype)initWithStyle:(ORKNumericAnswerStyle)style;

/**
 Returns an initialized numeric answer format using the specified style and unit designation.
 
 @param style       The style of the numeric answer (decimal or integer).
 @param unit        A string that displays a localized version of the unit designation.
 
 @return An initialized numeric answer format.
 */
- (instancetype)initWithStyle:(ORKNumericAnswerStyle)style
                         unit:(nullable NSString *)unit;

/**
Returns an initialized numeric answer format using the specified style, unit designation, and range
 values.
 
 @param style                   The style of the numeric answer (decimal or integer).
 @param unit                    A string that displays a localized version of the unit designation.
 @param minimum                 The minimum value to apply, or `nil` if none is specified.
 @param maximum                 The maximum value to apply, or `nil` if none is specified.

 @return An initialized numeric answer format.
 */
- (instancetype)initWithStyle:(ORKNumericAnswerStyle)style
                         unit:(nullable NSString *)unit
                      minimum:(nullable NSNumber *)minimum
                      maximum:(nullable NSNumber *)maximum;

/**
 Returns an initialized numeric answer format using the specified style, unit designation, range
 values, and precision.
 
 This method is the designated initializer.
 
 @param style                   The style of the numeric answer (decimal or integer).
 @param unit                    A string that displays a localized version of the unit designation.
 @param minimum                 The minimum value to apply, or `nil` if none is specified.
 @param maximum                 The maximum value to apply, or `nil` if none is specified.
 @param maximumFractionDigits   The maximum fraction digits, or `nil` if no maximum is specified.

 @return An initialized numeric answer format.
 */
- (instancetype)initWithStyle:(ORKNumericAnswerStyle)style
                         unit:(nullable NSString *)unit
                      minimum:(nullable NSNumber *)minimum
                      maximum:(nullable NSNumber *)maximum
        maximumFractionDigits:(nullable NSNumber *)maximumFractionDigits NS_DESIGNATED_INITIALIZER;

/**
 The style of numeric entry (decimal or integer). (read-only)
 */
@property (readonly) ORKNumericAnswerStyle style;

/**
 A string that displays a localized version of the unit designation next to the numeric value.
 (read-only)
 
 Examples of unit designations are days, lbs, and liters.
 The unit string is included in the `ORKNumericQuestionResult` object.
 */
@property (copy, readonly, nullable) NSString *unit;

/**
 The minimum allowed value for the numeric answer.
 
 The default value of this property is `nil`, which means that no minimum value is displayed.
 */
@property (copy, nullable) NSNumber *minimum;

/**
 The maximum allowed value for the numeric answer.
 
 The default value of this property is `nil`, which means that no maximum value is displayed.
 */
@property (copy, nullable) NSNumber *maximum;

/**
 The maximum number of fraction digits to the right of the decimal point for the
 numeric answer.
 
 The default value of this property is `nil`, which means that there's no maximum number of fraction
 digits.
 */
@property (copy, nullable) NSNumber *maximumFractionDigits;

/**
 The default numeric answer.
 */
@property (copy, nullable) NSNumber *defaultNumericAnswer;

/**
 A property that specifies whether the unit is hidden when the answer is empty.
 
 Defaults to `YES`.
 */
@property (assign) BOOL hideUnitWhenAnswerIsEmpty;

/**
The placeholder to dislpay when the answer is empty.
 
Overrides any specified step placeholder. Setting it to `nil` displays the default placeholeder.
*/
@property (copy, nullable) NSString *placeholder;

@end


/**
 The `ORKTimeOfDayAnswerFormat` class represents the answer format for questions that require users
 to enter a time of day.
 
 A time of day answer format produces an `ORKTimeOfDayQuestionResult` object.
 */
ORK_CLASS_AVAILABLE
@interface ORKTimeOfDayAnswerFormat : ORKAnswerFormat

/**
 Returns an initialized time of day answer format using the specified default value.
 
 This method is the designated initializer.
 
 @param defaultComponents   The default value with which to configure the picker.
 
 @return An initialized time of day answer format.
 */
- (instancetype)initWithDefaultComponents:(nullable NSDateComponents *)defaultComponents NS_DESIGNATED_INITIALIZER;

/**
 The default time of day to display in the picker. (read-only)
 
 Note that both the hour and minute components are observed. If the value of this property is `nil`,
 the picker displays the current time of day.
 */
@property (nonatomic, copy, readonly, nullable) NSDateComponents *defaultComponents;

/**
 The interval at which the date picker should display minutes.
 
 When the value of this property is not explicitly set, the picker defaults to an interval of
 one minute.
 */
@property (nonatomic) NSInteger minuteInterval;

@end


/**
 The style of date picker to use in an `ORKDateAnswerFormat` object.
 */
typedef NS_ENUM(NSInteger, ORKDateAnswerStyle) {

    /**
     The date and time question type asks participants to choose a time or a combination of date
     and time, from a picker.
     */
    ORKDateAnswerStyleDateAndTime,
    
    /**
     The date question type asks participants to choose a particular date from a picker.
     */
    ORKDateAnswerStyleDate
} ORK_ENUM_AVAILABLE;

/**
 The `ORKDateAnswerFormat` class represents the answer format for questions that require users
 to enter a date, or a date and time.
 
 A date answer format produces an `ORKDateQuestionResult` object.
 */
ORK_CLASS_AVAILABLE
@interface ORKDateAnswerFormat : ORKAnswerFormat

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/**
 Returns an initialized date answer format using the specified date style.
 
 @param style           The style of date answer, such as date, or date and time.
 
 @return An initialized date answer format.
 */
- (instancetype)initWithStyle:(ORKDateAnswerStyle)style;

/**
 Returns an initialized date answer format using the specified answer style and default date values.
 
 This method is the designated initializer.
 
 @param style           The style of date answer, such as date, or date and time.
 @param defaultDate     The default date to display. When the value of this parameter is `nil`, the
                            picker displays the current time.
 @param minimumDate     The minimum date that is accessible in the picker. If the value of this 
                            parameter is `nil`, there is no minimum.
 @param maximumDate     The maximum date that is accessible in the picker. If the value of this 
                            parameter is `nil`, there is no maximum.
 @param calendar        The calendar to use. If the value of this parameter is `nil`, the picker
                            uses the default calendar for the current locale.
 
 @return An initialized date answer format.
 */
- (instancetype)initWithStyle:(ORKDateAnswerStyle)style
                  defaultDate:(nullable NSDate *)defaultDate
                  minimumDate:(nullable NSDate *)minimumDate
                  maximumDate:(nullable NSDate *)maximumDate
                     calendar:(nullable NSCalendar *)calendar NS_DESIGNATED_INITIALIZER;

/**
 The style of date entry.
 */
@property (readonly) ORKDateAnswerStyle style;

/**
 The date to use as the default.
 
 The date is displayed in the user's time zone.
 When the value of this property is `nil`, the current time is used as the default.
 */
@property (copy, readonly, nullable) NSDate *defaultDate;

/**
 The minimum allowed date.
 
When the value of this property is `nil`, there is no minimum.
 */
@property (copy, readonly, nullable) NSDate *minimumDate;

/**
 The maximum allowed date.
 
 When the value of this property is `nil`, there is no maximum.
 */
@property (copy, readonly, nullable) NSDate *maximumDate;

/**
 The calendar to use in the picker.
 
 When the value of this property is `nil`, the picker uses the default calendar for the current
 locale.
 */
@property (copy, readonly, nullable) NSCalendar *calendar;

/**
 The interval at which the date picker should display minutes.
 
 When the value of this property is not explicitly set, the picker defaults to an interval of
 one minute.
 */
@property (nonatomic) NSInteger minuteInterval;

@end


/**
 The `ORKTextAnswerFormat` class represents the answer format for questions that collect a text
 response
 from the user.
 
 An `ORKTextAnswerFormat` object produces an `ORKTextQuestionResult` object.
 */
ORK_CLASS_AVAILABLE
@interface ORKTextAnswerFormat : ORKAnswerFormat

/**
 Returns an initialized text answer format using the regular expression.
 
 This method is one of the designated initializers.
 
 @param validationRegularExpression     The regular expression used to validate the text.
 @param invalidMessage                  The text presented to the user when invalid input is received.
 
 @return An initialized validated text answer format.
 */
- (instancetype)initWithValidationRegularExpression:(NSRegularExpression *)validationRegularExpression
                                     invalidMessage:(NSString *)invalidMessage NS_DESIGNATED_INITIALIZER;

/**
 Returns an initialized text answer format using the specified maximum string length.
 
 This method is one of the designated initializers.
 
 @param maximumLength   The maximum number of characters to accept. When the value of this parameter
                            is 0, there is no maximum.
 
 @return An initialized text answer format.
 */
- (instancetype)initWithMaximumLength:(NSInteger)maximumLength NS_DESIGNATED_INITIALIZER;

/**
 The regular expression used to validate user's input.
 
 The default value is nil. If set to nil, no validation will be performed.
 */
@property (nonatomic, copy, nullable) NSRegularExpression *validationRegularExpression;

/**
 The text presented to the user when invalid input is received.
 
 The default value is nil.
 */
@property (nonatomic, copy, nullable) NSString *invalidMessage;

/**
 The text to be used as an answer if user input is not mandatory.
 
 The default value is nil. If set to nil, user input is mandatory to answer.
 */
@property (nonatomic, copy, nullable) NSString *defaultTextAnswer;

/**
 The maximum length of the text users can enter.
 
 When the value of this property is 0, there is no maximum.
 */
@property NSInteger maximumLength;

/**
 A Boolean value indicating whether to expect more than one line of input.
 
 By default, the value of this property is `YES`.
 */
@property BOOL multipleLines;

/**
A Boolean that determines if the clear button should be hidden.
 
This only applies if multipleLines is set to YES.

This By default, the value of this property is `NO`.
*/
@property BOOL hideClearButton;

/**
A Boolean that determines if the word count label should be hidden.
 
This only applies if multipleLines is set to YES.

This By default, the value of this property is `NO`.
*/
@property BOOL hideCharacterCountLabel;

/**
 The autocapitalization type that applies to the user's input.
 
 By default, the value of this property is `UITextAutocapitalizationTypeSentences`.
 */
@property UITextAutocapitalizationType autocapitalizationType;

/**
 The autocorrection type that applies to the user's input.
 
 By default, the value of this property is `UITextAutocorrectionTypeDefault`.
 */
@property UITextAutocorrectionType autocorrectionType;

/**
 The spell checking type that applies to the user's input.
 
 By default, the value of this property is `UITextSpellCheckingTypeDefault`.
 */
@property UITextSpellCheckingType spellCheckingType;

/**
 The keyboard type that applies to the user's input.
 
 By default, the value of this property is `UIKeyboardTypeDefault`.
 */
@property UIKeyboardType keyboardType;

/**
 The semantic UITextContentType that applies to the user's input.
 
 If specified the system can improve keyboard suggestions to help with filling forms and other
 input. By default, the value of this property is `nil` meaning no specific type.
 */
@property (nonatomic, copy, nullable) UITextContentType textContentType;

/**
 The password generation rules to use for Automatic Secure Passwords.
 
 If specified, overrides the default passsword generation rules for fields with secureTextEntry.
 */
@property (nonatomic, copy, nullable) UITextInputPasswordRules *passwordRules API_AVAILABLE(ios(12));

/**
 Identifies whether the text object should hide the text being entered.
 
 By default, the value of this property is NO.
 */
@property (nonatomic,getter=isSecureTextEntry) BOOL secureTextEntry;

/**
 The placeholder to dislpay when the answer is empty.
 
 Overrides any specified step placeholder. Setting it to `nil` displays the default placeholeder.
  */
@property (copy, nullable) NSString *placeholder;

@end


/**
 The `ORKEmailAnswerFormat` class represents the answer format for questions that collect an email
 response from the user.
 
 An `ORKEmailAnswerFormat` object produces an `ORKTextQuestionResult` object.
 */
ORK_CLASS_AVAILABLE
@interface ORKEmailAnswerFormat : ORKAnswerFormat

/**
 Identifies whether this email answer format is being used as a username.
 
 For integration with iOS 12's password management functionality. Use this answer format if your
 username is also an email address, if it is not guaranteed to be an email address, use
 `ORKTextAnswerFormat` and set the `textContentType` to `UITextContentTypeUsername`.
 
 By default, the value of this property is NO.
 */
@property (nonatomic,getter=isUsernameField) BOOL usernameField;

@end


/**
 The `ORKLocationAnswerFormat` class represents the answer format for questions that collect a location response
 from the user.
 
 An `ORKLocationAnswerFormat` object produces an `ORKLocationQuestionResult` object.
 */
ORK_CLASS_AVAILABLE
@interface ORKLocationAnswerFormat : ORKAnswerFormat

/**
 Indicates whether or not the user's current location should be automatically entered the first time they tap on the input field.
 
 By default, this value is YES.
 */
@property (nonatomic, assign) BOOL useCurrentLocation;

/**
 The placeholder to dislpay when the answer is empty.
 
 Overrides any specified step placeholder. Setting it to `nil` displays the default placeholeder.
  */
@property (copy, nullable) NSString *placeholder;

@end

/**
 Defers to the supplied formatter to format the text.
*/
ORK_CLASS_AVAILABLE
@interface ORKCurrencyAnswerFormat : ORKNumericAnswerFormat

- (instancetype)initWithLocaleIdentifier:(nullable NSString *)localeIdentifier
                            currencyCode:(nullable NSString *)currencyCode;

@property (nonatomic, readonly) NSString *localeIdentifier;
@property (nonatomic, readonly) NSString *currencyCode;
@property (nonatomic, readonly) CurrencyFormatter *currencyFormatter;

@end


NS_ASSUME_NONNULL_END
