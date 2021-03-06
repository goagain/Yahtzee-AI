# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc

import game_pb2 as game__pb2


class GameStub(object):
    """Missing associated documentation comment in .proto file."""

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.Roll = channel.unary_unary(
                '/Game/Roll',
                request_serializer=game__pb2.RollRequest.SerializeToString,
                response_deserializer=game__pb2.RollResponse.FromString,
                )
        self.Reroll = channel.unary_unary(
                '/Game/Reroll',
                request_serializer=game__pb2.RerollRequest.SerializeToString,
                response_deserializer=game__pb2.RerollResponse.FromString,
                )
        self.Write = channel.unary_unary(
                '/Game/Write',
                request_serializer=game__pb2.WriteRequest.SerializeToString,
                response_deserializer=game__pb2.WriteResponse.FromString,
                )
        self.GetGameStatus = channel.unary_unary(
                '/Game/GetGameStatus',
                request_serializer=game__pb2.GetGameStatusRequest.SerializeToString,
                response_deserializer=game__pb2.GetGameStatusResponse.FromString,
                )


class GameServicer(object):
    """Missing associated documentation comment in .proto file."""

    def Roll(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Reroll(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Write(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def GetGameStatus(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_GameServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'Roll': grpc.unary_unary_rpc_method_handler(
                    servicer.Roll,
                    request_deserializer=game__pb2.RollRequest.FromString,
                    response_serializer=game__pb2.RollResponse.SerializeToString,
            ),
            'Reroll': grpc.unary_unary_rpc_method_handler(
                    servicer.Reroll,
                    request_deserializer=game__pb2.RerollRequest.FromString,
                    response_serializer=game__pb2.RerollResponse.SerializeToString,
            ),
            'Write': grpc.unary_unary_rpc_method_handler(
                    servicer.Write,
                    request_deserializer=game__pb2.WriteRequest.FromString,
                    response_serializer=game__pb2.WriteResponse.SerializeToString,
            ),
            'GetGameStatus': grpc.unary_unary_rpc_method_handler(
                    servicer.GetGameStatus,
                    request_deserializer=game__pb2.GetGameStatusRequest.FromString,
                    response_serializer=game__pb2.GetGameStatusResponse.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'Game', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class Game(object):
    """Missing associated documentation comment in .proto file."""

    @staticmethod
    def Roll(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/Game/Roll',
            game__pb2.RollRequest.SerializeToString,
            game__pb2.RollResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Reroll(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/Game/Reroll',
            game__pb2.RerollRequest.SerializeToString,
            game__pb2.RerollResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Write(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/Game/Write',
            game__pb2.WriteRequest.SerializeToString,
            game__pb2.WriteResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def GetGameStatus(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/Game/GetGameStatus',
            game__pb2.GetGameStatusRequest.SerializeToString,
            game__pb2.GetGameStatusResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)
